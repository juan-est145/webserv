/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerActionGenerator.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:01:38 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/29 12:11:10 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERACTIONGENERATOR_HPP
#define SERVERACTIONGENERATOR_HPP

#include "ResourceReq.hpp"
#include "PostUpload.hpp"
#include "Delete.hpp"
#include "RequestData.hpp"

namespace Webserv
{
	class ServerActionGenerator
	{
	public:
		static AServerAction *createServerAction(const struct RequestData &reqData, int socketFd);
		~ServerActionGenerator();
	};	
}

#endif