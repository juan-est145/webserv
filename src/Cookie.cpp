/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:33:20 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/19 20:36:44 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cookie.hpp"

namespace Webserv
{
	Cookie::Cookie(void) {}

	Cookie::Cookie(const Cookie &toCopy)
	{
		*this = toCopy;
	}

	Cookie &Cookie::operator=(const Cookie &toCopy)
	{
		(void)toCopy;
		return (*this);
	}

	struct CookieData Cookie::createCookie(const std::string &path, const std::string method, const std::string id) const
	{
		CookieData cookieData;
		
		cookieData._id = id.size() == 0 ? this->genCookieId() : id;
		cookieData._timeStamp = AuxFunc::getGmtTime();
		cookieData._path = path;
		cookieData._method = method;
		return (cookieData);
	}

	std::string Cookie::genCookieId(void) const
	{
		const char values[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		const int size = sizeof(values) / sizeof(values[0]);
		std::string id;
	
		srand(time(NULL));
		for (int i = 0; i < size / 2; i++)
			id += values[rand() % (size - 1)];
		return (id);
	}

	Cookie::~Cookie() {}
}
