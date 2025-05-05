/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:12:09 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/05 18:15:26 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unistd.h>
#include "Location.hpp"
#include "AuxFunc.hpp"

namespace Webserv
{
	class Location;

	class Cgi
	{
	private:
		typedef int cgiExtenIndex;
		typedef int urlSegmentIndex;

		const Location *_locationConf;
		std::string _interpreter;
		std::string _pathInfo;

		std::pair<cgiExtenIndex, urlSegmentIndex> selectCgiExtensions(const std::vector<std::string> &segmentedPath) const;
		void extractPathInfoAndInter(
			const std::pair<cgiExtenIndex, urlSegmentIndex> &indexes, 
			const std::string &path, 
			const std::vector<std::string> &segmentedPath
		);
		void findCgiFile(
			const std::string &path,
			const std::vector<std::string> segmentedPath,
			const std::pair<Cgi::cgiExtenIndex, Cgi::urlSegmentIndex> &indexes
		);

	public:
		Cgi(void);
		Cgi(const Location &location);
		Cgi(const Cgi &toCopy);
		Cgi &operator=(const Cgi &toCopy);
		bool canProcessAsCgi(const std::string &path);
		~Cgi();
	};
}

#endif