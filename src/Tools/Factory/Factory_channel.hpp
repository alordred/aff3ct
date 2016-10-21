#ifndef FACTORY_CHANNEL_HPP
#define FACTORY_CHANNEL_HPP

#include "Module/Channel/Channel.hpp"
#include "Tools/params.h"

#include "Factory.hpp"

template <typename R>
struct Factory_channel : public Factory
{
	static Channel<R>* build(const parameters &params, const R& sigma, const int size, mipp::vector<R>& H,
	                         const int seed = 0, const bool is_complex = false);
};

#endif /* FACTORY_CHANNEL_HPP */
