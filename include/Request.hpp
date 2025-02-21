/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:45 by juestrel          #+#    #+#             */
/*   Updated: 2025/02/21 10:12:08 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <exception>
#include "Logger.hpp"
#include "ResourceReq.hpp"

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
		
		
		Request(void);
		Request(int socketFd);
		Request(const Request &copy);
		Request &operator=(const Request &assign);
		void processReq(const char *buffer);
		const std::map<std::string, std::string> &getReqHeader(void) const;
		enum E_Method getMethod(void) const;
		const std::string &getPath(void) const;
		const std::string &getHttpVers(void) const;
		unsigned int getResCode(void) const;
		long getResourceSize(void) const;
		const std::string &getResourceContent(void) const;
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
		int _socketFd;
		enum E_Method _method;
		std::string _path;
		std::string _httpVers;
		unsigned int _resCode;
		ResourceReq _resourceReq;
		void extractHeaders(std::string &buffer);
		void extractReqHead(std::queue<std::string> &headers);
		void extractFirstHead(std::string &line);
		enum E_Method selectMethod(std::string &method);
	};
}

#endif