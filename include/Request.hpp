/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:45 by juestrel          #+#    #+#             */
/*   Updated: 2025/02/16 18:29:04 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <exception>
#include <sys/stat.h>
#include "Logger.hpp"

namespace Webserv
{
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
		enum E_ResourceType
		{
			REG_FILE,
			BIN_FILE,
			DIRECTORY,
			CGI,
		};
		struct S_Resource
		{
			std::string path;
			long size;
			enum E_ResourceType resourceType;
		};
		
		Request(void);
		Request(const Request &copy);
		Request &operator=(const Request &assign);
		void processReq(const char *buffer);
		const std::map<std::string, std::string> &getReqHeader(void) const;
		enum E_Method getMethod(void) const;
		const std::string &getPath(void) const;
		const std::string &getHttpVers(void) const;
		unsigned int getResCode(void) const;
		void setResCode(unsigned int resCode);
		const struct S_Resource &getResourceData(void) const;
		class RequestException : std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
		~Request();

	private:
		std::map<std::string, std::string> _reqHeader;
		enum E_Method _method;
		std::string _path;
		std::string _httpVers;
		unsigned int _resCode;
		struct S_Resource _resourceData;
		void extractHeaders(const char *buffer);
		void extractReqHead(std::queue<std::string> &headers);
		void extractFirstHead(std::string &line);
		std::string mapUriToResource(void);
		enum E_Method selectMethod(std::string &method);
	};
}

#endif