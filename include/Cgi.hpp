/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:12:09 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/04 18:26:50 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "Location.hpp"

namespace Webserv
{
	class Location;

	class Cgi
	{
	private:
		const Location *_locationConf;

		typedef int cgiExtenIndex;
		typedef int urlSegmentIndex;

		std::pair<int, int> selectCgiExtensions(const std::vector<std::string> &segmentedPath) const;
	public:
		Cgi(void);
		Cgi(const Location &location);
		bool isCgi(const std::string &path) const;
		~Cgi();
	};
}

#endif