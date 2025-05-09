/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceReq.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:26:04 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/09 17:55:04 by juestrel         ###   ########.fr       */
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
	private:
		void obtainResource(const ConfigServer *config, const Request &req);
		std::string chooseMime(const std::string &path) const;
		std::pair<std::string, std::string> binaryMimeSearch(
			std::pair<std::string, std::string> *mimes,
			int low, int high,
			std::string toFind) const;

	public:
		ResourceReq(void);
		ResourceReq(const std::string path);
		ResourceReq(const ResourceReq &copy);
		ResourceReq &operator=(const ResourceReq &assign);

		void processRequest(const ConfigServer *config, const Request &req);

		void setContent(const std::string &_content);
		~ResourceReq();
	};
}

#endif