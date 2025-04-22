/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceReq.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:26:04 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/22 13:19:25 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCEREQ_HPP
#define RESOURCEREQ_HPP

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <algorithm>
#include "AServerAction.hpp"
#include "Request.hpp"

namespace Webserv
{
	class ConfigServer;
	class Location;

	class ResourceReq: public AServerAction
	{
	public:
		enum E_ResourceType
		{
			REG_FILE,
			DIRECTORY,
			CGI,
		};

		ResourceReq(void);
		ResourceReq(const std::string path);
		ResourceReq(const ResourceReq &copy);
		ResourceReq &operator=(const ResourceReq &assign);

		void processRequest(const ConfigServer *config, const Request &req);

		void setContent(const std::string &_content);
		~ResourceReq();

	private:
		enum E_ResourceType _resourceType;

		void obtainResource(const ConfigServer *config, const Request &req);
		const Location &obtainLocationConf(const ConfigServer *config) const;
		std::string mapPathToResource(const Location &locationFile) const;
	};
}

#endif