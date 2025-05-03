/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:13:04 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/03 19:54:01 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cgi.hpp"

namespace Webserv
{
	Cgi::Cgi(): _locationConf(NULL) {}

	Cgi::Cgi(const Location &location): _locationConf(&location) {}

	bool Cgi::isCgi(const std::string &path) const
	{
		std::vector<std::string> segmentedPath;
		const std::string delimiter = "/";
		std::string copy(path);
		size_t pos = 0;
		const std::vector<std::string> cgiExten = this->_locationConf->getCgiExtension();

		while ((pos = copy.find(delimiter)) != std::string::npos)
		{
			segmentedPath.push_back(copy.substr(0, pos));
			copy.erase(0, pos + delimiter.size());
		}
		if (copy.size() > 0)
			segmentedPath.push_back(copy);
		for (size_t i = 0; i < segmentedPath.size(); i++)
		{
			for (size_t extenIndex = 0; extenIndex < cgiExten.size(); extenIndex++)
			{
				size_t startIndex = segmentedPath[i].rfind(cgiExten[extenIndex]);
				if (startIndex != std::string::npos && segmentedPath[i].substr(startIndex) == cgiExten[extenIndex])
				{
					std::cout << "Extension " << cgiExten[extenIndex] << " was found on key word " << segmentedPath[i] << std::endl;
					return (true);
				}
			}
		}
		
		return (true);
	}

	Cgi::~Cgi() {}
}