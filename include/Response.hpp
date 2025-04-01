/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 17:57:04 by mfuente-          #+#    #+#             */
/*   Updated: 2025/02/25 18:58:30 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include "Request.hpp"

namespace Webserv
{
	class Response
	{
	private:
        Request req;
	public:
        Response(void);
        Response(Request req);
        Response(Response &toCopy);
        Response &operator=(const Response &assign);
        ~Response();
    
	};

}
#endif