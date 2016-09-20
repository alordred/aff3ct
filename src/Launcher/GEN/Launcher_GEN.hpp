#ifndef LAUNCHER_GEN_HPP_
#define LAUNCHER_GEN_HPP_

#include "../Launcher.hpp"

template <typename B, typename R, typename Q>
class Launcher_GEN : public Launcher<B,R,Q>
{
public:
	Launcher_GEN(const int argc, const char **argv, std::ostream &stream = std::cout);
	virtual ~Launcher_GEN() {};

protected:
	virtual void build_args();
	virtual void store_args();
};

#endif /* LAUNCHER_GEN_HPP_ */
