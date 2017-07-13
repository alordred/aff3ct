#ifndef FACTORY_CRC_HPP
#define FACTORY_CRC_HPP

#include <string>
#include <sstream>

#include "Module/CRC/CRC.hpp"

#include "Tools/Arguments_reader.hpp"

#include "../Factory.hpp"

namespace aff3ct
{
namespace tools
{
struct Factory_CRC : public Factory
{
	struct parameters
	{
		int         K             = -1;

		std::string type          = "FAST";
		std::string poly          = "";
		bool        inc_code_rate = false;
		int         size          = 0;
		int         n_frames      = 1;
	};

	template <typename B = int>
	static module::CRC<B>* build(const parameters &params);

	static void build_args(Arguments_reader::arg_map &req_args, Arguments_reader::arg_map &opt_args);
	static void store_args(const Arguments_reader& ar, parameters &params);
	static void group_args(Arguments_reader::arg_grp& ar);

	static void header(params_list& head_crc, const parameters& params);
};
}
}

#endif /* FACTORY_CRC_HPP */