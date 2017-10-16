#include <algorithm>
#include <iostream>
#include <utility>
#include <sstream>
#include <vector>
#include <map>

#include "Tools/general_utils.h"
#include "Tools/Exception/exception.hpp"

#include "Factory.hpp"

using namespace aff3ct;
using namespace aff3ct::factory;

bool aff3ct::factory::exist(const arg_val_map &vals, const std::vector<std::string> &tags)
{
	return (vals.find(tags) != vals.end());
}

Factory::parameters
::parameters(const std::string name, const std::string short_name, const std::string prefix)
: name(name), short_name(short_name), prefix(prefix)
{
}

Factory::parameters
::~parameters()
{
}

std::string Factory::parameters
::get_name() const
{
	return this->name;
}
std::string Factory::parameters
::get_short_name() const
{
	return this->short_name;
}

std::string Factory::parameters
::get_prefix() const
{
	return this->prefix;
}

std::vector<std::string> Factory::parameters
::get_names() const
{
	std::vector<std::string> n;
	n.push_back(this->name);
	return n;
}
std::vector<std::string> Factory::parameters
::get_short_names() const
{
	std::vector<std::string> sn;
	sn.push_back(this->short_name);
	return sn;
}

std::vector<std::string> Factory::parameters
::get_prefixes() const
{
	std::vector<std::string> p;
	p.push_back(this->prefix);
	return p;
}

std::pair<arg_map, arg_map> Factory
::get_description(const std::vector<Factory::parameters*> &params)
{
	std::pair<arg_map, arg_map> args;
	for (auto *p : params)
		p->get_description(args.first, args.second);

	return args;
}

void Factory
::store(std::vector<Factory::parameters*> &params, const arg_val_map &vals)
{
	for (auto *p : params)
		p->store(vals);
}

aff3ct::factory::arg_grp Factory
::create_groups(const std::vector<Factory::parameters*> &params)
{
	// create groups of arguments
	aff3ct::factory::arg_grp grps;
	for (auto *p : params)
	{
		auto prefixes    = p->get_prefixes   ();
		auto short_names = p->get_short_names();

		if (prefixes.size() != short_names.size())
		{
			std::stringstream message;
			message << "'prefixes.size()' has to be equal to 'short_names.size()' ('prefixes.size()' = "
			        << prefixes.size() << ", 'short_names.size()' = " << short_names.size() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		for (size_t i = 0; i < prefixes.size(); i++)
			grps.push_back({prefixes[i], short_names[i] + " parameter(s)"});
	}

	return grps;
}

void aff3ct::factory::Header::print_parameters(std::string grp_key, std::string grp_name, header_list header,
                                               int max_n_chars, std::ostream& stream)
{
	auto key = tools::string_split(grp_key, '-');

	if (key.size() == 1)
	{
		stream << "# * " << tools::format(grp_name, tools::Style::BOLD | tools::Style::UNDERLINED) << " ";
		for (auto i = 0; i < 46 - (int)grp_name.length(); i++) std::cout << "-";
		stream << std::endl;
	}
	else if (key.size() > 1)
	{
		stream << "#    " << tools::format(grp_name, tools::Style::BOLD | tools::Style::UNDERLINED) << " ";
		for (auto i = 0; i < 45 - (int)grp_name.length(); i++) std::cout << "-";
		stream << std::endl;
	}

	std::vector<std::string> dup;
	for (auto i = 0; i < (int)header.size(); i++)
	{
		if (std::find(dup.begin(), dup.end(), header[i].first + header[i].second) == dup.end())
		{
			stream << "#    ** " << tools::style(header[i].first, tools::Style::BOLD);
			for (auto j = 0; j < max_n_chars - (int)header[i].first.length(); j++) stream << " ";
			stream << " = " << header[i].second << std::endl;

			dup.push_back(header[i].first + header[i].second);
		}
	}
}

void aff3ct::factory::Header::print_parameters(const std::vector<Factory::parameters*> &params, std::ostream& stream)
{
	int max_n_chars = 0;
	for (auto *p : params)
	{
		std::map<std::string,aff3ct::factory::header_list> headers;
		p->get_headers(headers, true);

		for (auto &h : headers)
			aff3ct::factory::Header::compute_max_n_chars(h.second, max_n_chars);
	}

	std::vector<aff3ct::factory::header_list> dup_h;
	std::vector<std::string                 > dup_n;
	for (auto *p : params)
	{
		std::map<std::string,aff3ct::factory::header_list> headers;
		p->get_headers(headers, true);

		auto prefixes = p->get_prefixes();
		auto short_names = p->get_short_names();

		if (prefixes.size() != short_names.size())
		{
			std::stringstream message;
			message << "'prefixes.size()' has to be equal to 'short_names.size()' ('prefixes.size()' = "
			        << prefixes.size() << ", 'short_names.size()' = " << short_names.size() << ").";
			throw tools::runtime_error(__FILE__, __LINE__, __func__, message.str());
		}

		for (size_t i = 0; i < prefixes.size(); i++)
		{
			auto h = headers[prefixes[i]];
			auto n = short_names[i];
			if (h.size() && (std::find(dup_h.begin(), dup_h.end(), h) == dup_h.end() ||
			                 std::find(dup_n.begin(), dup_n.end(), n) == dup_n.end()))
			{
				aff3ct::factory::Header::print_parameters(prefixes[i], n, h, max_n_chars);

				dup_h.push_back(h);
				dup_n.push_back(n);
			}
		}
	}
}

void aff3ct::factory::Header::compute_max_n_chars(const header_list& header, int& max_n_chars)
{
	for (unsigned i = 0; i < header.size(); i++)
		max_n_chars = std::max(max_n_chars, (int)header[i].first.length());
}
