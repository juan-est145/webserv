/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:33:20 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/19 20:06:35 by juestrel         ###   ########.fr       */
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

	void Cookie::createCookie(void) const
	{
		std::string id = this->genCookieId();
		(void)id;
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
