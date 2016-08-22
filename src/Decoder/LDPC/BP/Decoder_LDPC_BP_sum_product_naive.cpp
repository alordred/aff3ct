#include <limits>

#include "../../../Tools/bash_tools.h"
#include "../../decoder_functions.h"

#include "Decoder_LDPC_BP_sum_product_naive.hpp"

constexpr int C_to_V_max = 15; // saturation value for the LLRs/extrinsics

template <typename B, typename R>
Decoder_LDPC_BP_sum_product_naive<B,R>
::Decoder_LDPC_BP_sum_product_naive(const int &N, const int &K, const int& n_ite,
                                    const std::vector<unsigned char> &n_variables_per_parity,
                                    const std::vector<unsigned char> &n_parities_per_variable,
                                    const std::vector<unsigned int > &transpose)
: Decoder_LDPC_BP_naive<B,R>(N, K, n_ite, n_variables_per_parity, n_parities_per_variable, transpose)
{
}

template <typename B, typename R>
Decoder_LDPC_BP_sum_product_naive<B,R>
::~Decoder_LDPC_BP_sum_product_naive()
{
}

// sum-product implementation
template <typename B, typename R>
bool Decoder_LDPC_BP_sum_product_naive<B,R>
::BP_process()
{
	auto syndrome = 0;
	auto transpose_ptr = this->transpose.data();

	R values[32]; // lets suppose that 32 >= length is always true...
	for (auto i = 0; i < this->n_C_nodes; i++)
	{
		const auto length = this->n_variables_per_parity[i];

		auto sign =    0;
		auto sum  = (R)0;

		// accumulate the incoming information in CN
		for (auto j = 0; j < length; j++)
		{
			const auto value  = this->V_to_C[transpose_ptr[j]];
			const auto v_abs  = (R)std::abs(value);
			const auto res    = (R)std::tanh(v_abs * (R)0.5);
			const auto c_sign = std::signbit(value) ? -1 : 0;

			sign ^= c_sign;
			sum  += res;
			values[j] = res;
		}

		// regenerate the CN outcoming values
		for (auto j = 0; j < length; j++)
		{
			const auto value   = this->V_to_C[transpose_ptr[j]];
			const auto v_sig   = sign ^ (std::signbit(value) ? -1 : 0);
			const auto tmp1    = sum / values[j];
			const auto tmp2    = (tmp1 >= 1) ? 0.9999 : tmp1; // saturation to avoid infinite numbers
			const auto v_res   = (R)2.0 * std::atanh(tmp2);
			const auto v_to_st = (R)std::copysign(v_res, v_sig);

			this->C_to_V[transpose_ptr[j]] = v_to_st;
		}

		transpose_ptr += length;
		syndrome = syndrome || sign;
	}

	return (syndrome == 0);
}

// ==================================================================================== explicit template instantiation 
#include "../../../Tools/types.h"
#ifdef MULTI_PREC
template class Decoder_LDPC_BP_sum_product_naive<B_8,Q_8>;
template class Decoder_LDPC_BP_sum_product_naive<B_16,Q_16>;
template class Decoder_LDPC_BP_sum_product_naive<B_32,Q_32>;
template class Decoder_LDPC_BP_sum_product_naive<B_64,Q_64>;
#else
template class Decoder_LDPC_BP_sum_product_naive<B,Q>;
#endif
// ==================================================================================== explicit template instantiation