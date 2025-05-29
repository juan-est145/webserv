/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServer.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:49:22 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/29 12:53:04 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/IServer.hpp"

namespace Webserv
{
	const char *IServer::ServerException::what(void) const throw()
	{
		return ("The server found a problem and must stop now");
	}
}