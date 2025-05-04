/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:13:04 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/04 18:55:02 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cgi.hpp"

namespace Webserv
{
	Cgi::Cgi() : _locationConf(NULL) 
	{
		this->_interpreter = "";
	}

	Cgi::Cgi(const Location &location) : _locationConf(&location) 
	{
		this->_interpreter = "";
	}

	Cgi::Cgi(const Cgi &toCopy): _locationConf(toCopy._locationConf)
	{
		*this = toCopy;
	}

	Cgi &Cgi::operator=(const Cgi &toCopy)
	{
		if (this != &toCopy)
		{
			this->_interpreter = toCopy._interpreter;
			this->_locationConf = toCopy._locationConf;
		}
		return (*this);
	}

	bool Cgi::isCgi(const std::string &path)
	{
		std::vector<std::string> segmentedPath;
		const std::string delimiter = "/";
		std::string copy(path);
		size_t pos = 0;

		while ((pos = copy.find(delimiter)) != std::string::npos)
		{
			segmentedPath.push_back(copy.substr(0, pos));
			copy.erase(0, pos + delimiter.size());
		}
		if (copy.size() > 0)
			segmentedPath.push_back(copy);
		const std::pair<cgiExtenIndex, urlSegmentIndex> indexes = this->selectCgiExtensions(segmentedPath);
		if (indexes.first == -1 || indexes.second == -1)
			return (false);
		this->_interpreter = this->_locationConf->getCgiPath()[indexes.first];
		return (true);
	}

	std::pair<int, int> Cgi::selectCgiExtensions(const std::vector<std::string> &segmentedPath) const
	{
		std::pair<cgiExtenIndex, urlSegmentIndex> indexes = std::make_pair(-1, -1);
		const std::vector<std::string> cgiExten = this->_locationConf->getCgiExtension();

		for (size_t i = 0; i < segmentedPath.size(); i++)
		{
			for (size_t extenIndex = 0; extenIndex < cgiExten.size(); extenIndex++)
			{
				size_t startIndex = segmentedPath[i].rfind(cgiExten[extenIndex]);
				if (startIndex != std::string::npos && segmentedPath[i].substr(startIndex) == cgiExten[extenIndex])
				{
					indexes.first = extenIndex;
					indexes.second = i;
					return (indexes);
				}
			}
		}
		return (indexes);
	}

	Cgi::~Cgi() {}
}