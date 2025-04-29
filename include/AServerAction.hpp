/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerAction.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:51:54 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/29 18:21:08 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASERVERACTION_HPP
#define ASERVERACTION_HPP

#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <sys/stat.h>
#include <fstream>
#include "AuxFunc.hpp"
#include "ConfigServer.hpp"
#include "Location.hpp"
#include "Request.hpp"

namespace Webserv
{
	class ResourceReq;
	class ConfigServer;
	class Request;

	class AServerAction
	{
	protected:
		const std::string _path;
		std::string _content;
		long _size;
		unsigned int _resCode;

		void processHttpError(const ConfigServer *config);
		void readResource(const std::string &path);
		void isMethodAllowed(std::map<std::string, bool>::const_iterator &methodIter, const Location &locationFile, const Request &req);

	public:
		AServerAction(void);
		AServerAction(const std::string path);
		AServerAction(const AServerAction &toCopy);
		AServerAction &operator=(const AServerAction &toCopy);

		virtual void processRequest(const ConfigServer *config, const Request &req) = 0;

		const std::string &getPath(void) const;
		const std::string &getContent(void) const;
		long getSize(void) const;
		unsigned int getResCode(void) const;

		void setRescode(unsigned int resCode);

		virtual ~AServerAction();

		class HttpException : public std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
	};
}

#endif