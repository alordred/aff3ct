#ifndef FACTORY_PUNCTURER_POLAR_HPP
#define FACTORY_PUNCTURER_POLAR_HPP

#include "Tools/Code/Polar/Frozenbits_generator/Frozenbits_generator.hpp"

#include "Module/Puncturer/Polar/Puncturer_polar_wangliu.hpp"

#include "../Puncturer.hpp"

namespace aff3ct
{
namespace factory
{
extern const std::string Puncturer_polar_name;
extern const std::string Puncturer_polar_prefix;
struct Puncturer_polar : public Puncturer
{
	class parameters : public Puncturer::parameters
	{
	public:
		// ------------------------------------------------------------------------------------------------- PARAMETERS
		// empty

		// ---------------------------------------------------------------------------------------------------- METHODS
		explicit parameters(const std::string &p = Puncturer_polar_prefix);
		virtual ~parameters();
		Puncturer_polar::parameters* clone() const;

		// parameters construction
		void get_description(arg_map &req_args, arg_map &opt_args                              ) const;
		void store          (const arg_val_map &vals                                           );
		void get_headers    (std::map<std::string,header_list>& headers, const bool full = true) const;

		// builder
		template <typename B = int, typename Q = float>
		module::Puncturer_polar_wangliu<B,Q>* build(const tools::Frozenbits_generator &fb_generator) const;
	};

	template <typename B = int, typename Q = float>
	static module::Puncturer_polar_wangliu<B,Q>* build(const parameters                  &params,
	                                                   const tools::Frozenbits_generator &fb_generator);
};
}
}

#endif /* FACTORY_PUNCTURER_POLAR_HPP */
