/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerAction.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:51:54 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/20 08:34:43 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASERVERACTION_HPP
#define ASERVERACTION_HPP

// The timeout is defined in seconds. A default value of 86400 corresponds to 24 hours
#ifndef COOKIE_TIMEOUT
#define COOKIE_TIMEOUT 86400
#endif

#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <sys/stat.h>
#include <fstream>
#include <ctime>
#include "AuxFunc.hpp"
#include "ConfigServer.hpp"
#include "Location.hpp"
#include "ConfigServer.hpp"
#include "FirstHeader.hpp"
#include "Cgi.hpp"
#include "Cookie.hpp"

namespace Webserv
{
	class ConfigServer;
	class Location;
	class AuxFunc;
	class Request;

	class AServerAction
	{
	protected:
		const std::string _path;
		std::string _content;
		long _size;
		unsigned int _resCode;
		std::map<std::string, std::string> _resHeaders;
		struct CookieData _cookie;

		void processHttpError(const ConfigServer *config);
		void readResource(const std::string &path);
		const Location &obtainLocationConf(const ConfigServer *config) const;
		void isMethodAllowed(const Location &locationFile, const std::string &reqMethod);
		bool isCgi(
			const Location &locationFile,
			const std::string &path,
			const std::map<std::string, std::string> &reqHeader,
			const ConfigServer *config,
			const struct firstHeader &firstHeader,
			const std::string &body);
		virtual void redirect(const std::string &uri, const ConfigServer *config) = 0;
		void handleCookies(const std::map<std::string, std::string> &reqHeaders,
						   const std::string &path,
						   const std::string &method,
						   const std::map<std::string, struct CookieData> &sessions);
		std::string cookieSearch(
			const std::map<std::string, struct CookieData> &sessions,
			const std::map<std::string, std::string> &reqHeaders) const;

		void setContentType(const std::string &mime);
		void setContentLength(long size);

	public:
		AServerAction(void);
		AServerAction(const std::string path);
		AServerAction(const AServerAction &toCopy);
		AServerAction &operator=(const AServerAction &toCopy);

		virtual void processRequest(
			const ConfigServer *config,
			const Request &req,
			const std::map<std::string, Webserv::CookieData> &sessions) = 0;
		void prepareDirectErrCode(const ConfigServer *config, unsigned int errCode);

		const std::string &getPath(void) const;
		const std::string &getContent(void) const;
		long getSize(void) const;
		unsigned int getResCode(void) const;
		const std::map<std::string, std::string> &getResHeaders(void) const;
		std::string getMime(void) const;
		std::string getLocation(void) const;
		std::string getAllow(void) const;
		const struct CookieData &getCookie(void) const;

		void setRescode(unsigned int resCode);

		virtual ~AServerAction();

		class HttpException : public std::exception
		{
		private:
			unsigned int _resCode;

		public:
			HttpException(unsigned int resCode);
			unsigned int getResCode(void) const;
			virtual const char *what(void) const throw();
		};
	};
}

#endif