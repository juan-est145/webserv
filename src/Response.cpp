/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:14:44 by mfuente-          #+#    #+#             */
/*   Updated: 2025/02/25 19:03:04 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

namespace Webserv
{
	Response::Response(void) {}

    Response::Response(Request req)
    {
        this->req = req;
    }
	Response::Response(Response &toCopy)
	{
		*this = toCopy;
	}

	Response &Response::operator=(const Response &assign)
	{
		(void)assign;
		return *this;
	}
	Response::~Response(void) {}

}