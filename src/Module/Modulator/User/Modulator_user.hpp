#ifndef MODULATOR_USER_HPP_
#define MODULATOR_USER_HPP_

#include <complex>
#include <vector>

#include "Tools/Perf/MIPP/mipp.h"
#include "Tools/Math/max.h"

#include "../Modulator.hpp"

template <typename B, typename R, typename Q, proto_max<Q> MAX>
class Modulator_user : public Modulator<B,R,Q>
{
private:
	const int bits_per_symbol;
	const int nbr_symbols;
	const R sigma;
	R sqrt_es;
	const bool disable_sig2;
	mipp::vector<std::complex<R>> constellation;

public:
	Modulator_user(const int N, mipp::vector<R> &H, const int bits_per_symbol = 2, const std::string const_path ="", const R sigma = 1.0,
	               const bool disable_sig2 = false, const int n_frames = 1, const std::string name = "Modulator_user");
	virtual ~Modulator_user();

	virtual void   modulate(const mipp::vector<B>& X_N1,                              mipp::vector<R>& X_N2);
	virtual void demodulate(const mipp::vector<Q>& Y_N1,                              mipp::vector<Q>& Y_N2);
	virtual void demodulate(const mipp::vector<Q>& Y_N1, const mipp::vector<Q>& Y_N2, mipp::vector<Q>& Y_N3);

	int get_buffer_size_after_modulation(const int N);
};

#include "Modulator_user.hxx"

#endif // MODULATOR_USER_HPP_
