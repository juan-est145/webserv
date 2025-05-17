/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:45 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/17 17:22:17 by juestrel         ###   ########.fr       */
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
#include <utility>
#include "Logger.hpp"
#include "ResourceReq.hpp"
#include "PostUpload.hpp"
#include "FirstHeader.hpp"

namespace Webserv
{
	class ConfigServer;
	class AServerAction;

	class Request
	{
	private:
		int _socketFd;
		bool _ready;
		std::map<std::string, std::string> _reqHeader;
		std::string _reqBody;
		struct firstHeader _firstHeader;
		AServerAction *_serverAction;
		ConfigServer *_configuration;

		void extractHeaders(std::string &buffer);
		void extractReqHead(std::queue<std::string> &headers);
		void extractFirstHead(std::string &line);
		std::pair<std::string, enum method> selectMethod(std::string &method);
		void selectConfiguration(const std::vector<ConfigServer> &configs);
		void extractUrlAndQuery(const std::string &path);
		void dechunkBody(std::string &strBuff);
		bool isEndChunk(const std::string &strBuff);

	public:
		typedef std::map<std::string, std::string>::const_iterator T_reqHeadIter;

		Request(void);
		Request(int socketFd);
		Request(const Request &copy);
		Request &operator=(const Request &assign);

		void readReq(const char *buffer, size_t bufSize);
		void handleReq(const std::vector<ConfigServer> &configs);

		const std::map<std::string, std::string> &getReqHeader(void) const;
		std::pair<std::string, enum method> getMethod(void) const;
		const std::string &getPath(void) const;
		const std::string &getHttpVers(void) const;
		unsigned int getResCode(void) const;
		const std::string &getReqBody(void) const;
		long getResourceSize(void) const;
		const std::string &getResourceContent(void) const;
		const std::string &getResourceMime(void) const;
		const struct firstHeader &getFirstHeader(void) const;
		const std::string &getLocation(void) const;
		const std::string &getAllow(void) const;

		void setResCode(unsigned int resCode);
		std::size_t setReqBody(std::string &body);

		class RequestException : std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
		~Request();
	};
}

#endif