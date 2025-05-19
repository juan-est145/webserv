/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:33:22 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/19 19:34:58 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COOKIE_HPP
#define COOKIE_HPP

namespace Webserv
{
	class Cookie
	{
	private:
	public:
		Cookie(void);
		Cookie(const Cookie &toCopy);
		Cookie &operator=(const Cookie &toCopy);
		~Cookie();
	};
}

#endif
