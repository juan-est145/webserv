/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:45 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/19 17:25:20 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>

#include <queue>
#include <exception>
#include <cstdlib>
#include <vector>
#include "ConfigServer.hpp"
#include "Logger.hpp"
#include "ResourceReq.hpp"
#include "PostUpload.hpp"

namespace Webserv
{
	class ConfigServer;

	class Request
	{
	public:
		enum E_Method
		{
			GET = 0,
			POST = 1,
			DELETE = 2,
			UNKNOWN = 3,
		};
		typedef std::map<std::string, std::string>::const_iterator T_reqHeadIter;

		Request(void);
		Request(int socketFd);
		Request(const Request &copy);
		Request &operator=(const Request &assign);

		void readReq(const char *buffer, size_t bufSize);
		void handleReq(const std::vector<ConfigServer> &configs);

		const std::map<std::string, std::string> &getReqHeader(void) const;
		enum E_Method getMethod(void) const;
		const std::string &getPath(void) const;
		const std::string &getHttpVers(void) const;
		unsigned int getResCode(void) const;
		const std::string &getReqBody(void) const;
		long getResourceSize(void) const;
		const std::string &getResourceContent(void) const;

		void setResCode(unsigned int resCode);
		std::size_t setReqBody(std::string &body);

		class RequestException : std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
		~Request();

	private:
		int _socketFd;
		std::map<std::string, std::string> _reqHeader;
		std::string _reqBody;
		enum E_Method _method;
		std::string _path;
		std::string _httpVers;
		unsigned int _resCode;
		ResourceReq _resourceReq;
		ConfigServer *_configuration;

		void extractHeaders(std::string &buffer);
		void extractReqHead(std::queue<std::string> &headers);
		void extractFirstHead(std::string &line);
		enum E_Method selectMethod(std::string &method);
		void selectConfiguration(const std::vector<ConfigServer> &configs);
	};
}

#endif