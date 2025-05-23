/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:12:09 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/18 12:07:44 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#ifndef PIPE_READ
#define PIPE_READ 0
#endif

#ifndef PIPE_WRITE
#define PIPE_WRITE 1
#endif

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <exception>
#include "Location.hpp"
#include "AuxFunc.hpp"
#include "ConfigServer.hpp"
#include "FirstHeader.hpp"

namespace Webserv
{
	class Location;

	class Cgi
	{
	private:
		typedef int cgiExtenIndex;
		typedef int urlSegmentIndex;

		const Location *_locationConf;
		std::string _interpreter;
		std::string _pathInfo;

		std::vector<std::string> obtainSegmentedPath(const std::string &path) const;
		bool isValidCgiRoute(
			std::string &path,
			std::vector<std::string> &segmentedPath,
			std::pair<cgiExtenIndex, urlSegmentIndex> &indexes) const;
		std::pair<cgiExtenIndex, urlSegmentIndex> selectCgiExtensions(const std::vector<std::string> &segmentedPath) const;
		void extractPathInfoAndInter(
			const std::pair<cgiExtenIndex, urlSegmentIndex> &indexes,
			const std::string &path,
			const std::vector<std::string> &segmentedPath);
		std::string findCgiFile(
			const std::string &path,
			const std::vector<std::string> segmentedPath,
			const std::pair<Cgi::cgiExtenIndex, Cgi::urlSegmentIndex> &indexes) const;
		void execCgi(
			const std::string &path,
			const std::string &localPath,
			const std::map<std::string, std::string> &headers,
			std::string &content,
			const ConfigServer *config,
			const struct firstHeader &firstHeader,
			const std::string &body) const;
		void childProcess(
			int pipeFd[2],
			const std::string &path,
			const std::string &localPath,
			const std::string &body,
			const std::map<std::string, std::string> &headers,
			const ConfigServer *config,
			const struct firstHeader &firstHeader) const;
		void addHeaderValue(
			std::string &env,
			std::string errorValue,
			const std::string &searchValue,
			const std::map<std::string, std::string> &headers) const;
		void parentProcess(int *pipeFd, const std::string &body, pid_t &pid, std::string &content) const;

	public:
		Cgi(void);
		Cgi(const Location &location);
		Cgi(const Cgi &toCopy);
		Cgi &operator=(const Cgi &toCopy);

		bool canProcessAsCgi(
			const std::string &path,
			const std::map<std::string, std::string> &headers,
			std::string &content,
			const ConfigServer *config,
			const struct firstHeader &firstHeader,
			const std::string &body);

		~Cgi();

		class NotFoundException : public std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};

		class CgiErrorException : public std::exception
		{
		public:
			virtual const char *what(void) const throw();
		};
	};
}

#endif