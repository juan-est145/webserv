/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 19:33:20 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/19 19:37:30 by juestrel         ###   ########.fr       */
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
	}

	Cookie::~Cookie() {}
}
