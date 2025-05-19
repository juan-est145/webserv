/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:33:22 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/19 20:36:41 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOKIE_HPP
#define COOKIE_HPP

#include <string>
#include <ctime>
#include <cstdlib>
#include "AuxFunc.hpp"

namespace Webserv
{
	struct CookieData
	{
		std::string _id;
		std::string _timeStamp;
		std::string _method;
		std::string _path;
	};

	class Cookie
	{
	private:
		std::string genCookieId(void) const;

	public:
		Cookie(void);
		Cookie(const Cookie &toCopy);
		Cookie &operator=(const Cookie &toCopy);

		struct CookieData createCookie(const std::string &path, const std::string method, const std::string id = "") const;

		~Cookie();
	};
}

#endif
