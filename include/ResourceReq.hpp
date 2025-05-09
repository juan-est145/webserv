/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceReq.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:26:04 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/08 19:22:15 by juestrel         ###   ########.fr       */
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
#include <utility>
#include "AServerAction.hpp"
#include "Request.hpp"

namespace Webserv
{
	class Request;

	class ResourceReq : public AServerAction
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
		std::string chooseMime(const std::string &path) const;
		std::pair<std::string, std::string> binaryMimeSearch(
			std::pair<std::string, std::string> *mimes,
			int low, int high,
			std::string toFind
		) const;
	};
}

#endif