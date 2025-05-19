/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:33:22 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/19 19:53:50 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOKIE_HPP
#define COOKIE_HPP

#include <string>
#include <ctime>
#include <cstdlib>

namespace Webserv
{
	class Cookie
	{
	private:
		std::string genCookieId(void) const;
	public:
		Cookie(void);
		Cookie(const Cookie &toCopy);
		Cookie &operator=(const Cookie &toCopy);

		void createCookie(void) const;

		~Cookie();
	};
}

#endif
