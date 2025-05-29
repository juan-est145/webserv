/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerActionGenerator.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:05:43 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/29 12:40:25 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ServerActionGenerator.hpp"

namespace Webserv
{
	AServerAction *ServerActionGenerator::createServerAction(const struct RequestData &reqData, int socketFd)
	{
		if (reqData._firstHeader.method.second == POST)
			return new Webserv::PostUpload(reqData._reqBody, reqData._firstHeader.path, socketFd);
		else if (reqData._firstHeader.method.second == DELETE)
			return new Webserv::Delete(reqData._firstHeader.path, socketFd);
		return new Webserv::ResourceReq(reqData._firstHeader.path, socketFd);
	}
}