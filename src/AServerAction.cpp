/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerAction.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:05:50 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/20 08:31:27 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/AServerAction.hpp"

namespace Webserv
{
	AServerAction::AServerAction(void) : _path("")
	{
		this->_content = "";
		this->_size = -1;
		this->_resCode = 200;
		this->_resHeaders["Content-Type"] = "text/plain";
	}

	AServerAction::AServerAction(const std::string path) : _path(path)
	{
		this->_content = "";
		this->_size = -1;
		this->_resCode = 200;
		this->_resHeaders["Content-Type"] = "text/plain";
	}

	AServerAction::AServerAction(const AServerAction &toCopy)
	{
		*this = toCopy;
	}

	AServerAction &AServerAction::operator=(const AServerAction &toCopy)
	{
		if (this != &toCopy)
		{
			this->_content = toCopy._content;
			this->_size = toCopy._size;
			this->_resCode = toCopy._resCode;
			this->_resHeaders = toCopy._resHeaders;
		}
		return (*this);
	}

	void AServerAction::prepareDirectErrCode(const ConfigServer *config, unsigned int errCode)
	{
		this->_resCode = errCode;
		this->processHttpError(config);
		this->setContentType("text/html");
	}

	void AServerAction::processHttpError(const ConfigServer *config)
	{
		struct stat fileStat;
		std::map<short, std::string>::const_iterator errorPage = config->getErrorPages().find(this->_resCode);
		std::string localPath = "www/errorPages/error" + AuxFunc::ft_itoa(this->_resCode) + ".html";

		if (errorPage->second.length() > 0)
			localPath = config->getRoot() + errorPage->second;
		if (stat(localPath.c_str(), &fileStat) == -1 || !S_ISREG(fileStat.st_mode))
			throw std::exception();
		this->_size = fileStat.st_size;
		this->setContentLength(this->_size);
		this->readResource(localPath);
	}

	void AServerAction::readResource(const std::string &path)
	{
		std::ifstream resource;
		char *buffer = NULL;

		resource.open(path.c_str(), std::ios::in);
		if (!resource.is_open())
		{
			throw Webserv::AServerAction::HttpException(500);
		}
		buffer = new char[this->_size + 1];
		resource.read(buffer, this->_size);
		buffer[this->_size] = '\0';
		this->_content = buffer;
		delete[] buffer;
	}

	const Location &AServerAction::obtainLocationConf(const ConfigServer *config) const
	{
		const std::vector<Webserv::Location> locations = config->getLocations();
		unsigned int locIndex = 0;
		unsigned int matchingChars = 0;
		unsigned int maxMatch = 0;

		for (unsigned int i = 0; i < locations.size(); i++)
		{
			matchingChars = 0;
			std::string path = locations[i].getPath();
			for (unsigned int letter = 0; letter < std::min(path.size(), this->_path.size()); letter++)
			{
				if (path[letter] != this->_path[letter])
					break;
				matchingChars++;
			}
			if (matchingChars > maxMatch)
			{
				maxMatch = matchingChars;
				locIndex = i;
			}
		}
		return (config->getLocations()[locIndex]);
	}

	void AServerAction::isMethodAllowed(const Location &locationFile, const std::string &reqMethod)
	{
		const std::map<std::string, bool> methods = locationFile.getMethods();
		std::map<std::string, bool>::const_iterator it = methods.find(reqMethod);
		std::vector<std::string> allowedMethods;

		if (it != locationFile.getMethods().end() && it->second)
			return;
		for (it = methods.begin(); it != methods.end(); it++)
		{
			if (it->second)
				allowedMethods.push_back(it->first);
		}
		for (unsigned int i = 0; i < allowedMethods.size(); i++)
			this->_resHeaders["Allow"] += i < allowedMethods.size() - 1 ? allowedMethods[i] + ", " : allowedMethods[i];
		throw Webserv::AServerAction::HttpException(405);
	}

	bool AServerAction::isCgi(
		const Location &locationFile,
		const std::string &path,
		const std::map<std::string, std::string> &reqHeader,
		const ConfigServer *config,
		const struct firstHeader &firstHeader,
		const std::string &body)
	{
		Cgi cgi(locationFile);
		try
		{
			if (cgi.canProcessAsCgi(path, reqHeader, this->_content, config, firstHeader, body))
			{
				this->_size = this->_content.size();
				this->setContentLength(this->_size);
				this->setContentType("text/html");
				return (true);
			}
		}
		catch (const Webserv::Cgi::NotFoundException &e)
		{
			throw Webserv::AServerAction::HttpException(404);
		}
		catch (const Webserv::Cgi::CgiErrorException &e)
		{
			throw Webserv::AServerAction::HttpException(500);
		}
		return (false);
	}

	void AServerAction::handleCookies(
		const std::map<std::string, std::string> &reqHeaders,
		const std::string &path,
		const std::string &method,
		const std::map<std::string, struct CookieData> &sessions)
	{
		// TO DO: We need to know wether the cookie id has just been created or not, and if the client sends a cookie id,
		// but it is not present in the session, we must not store it

		// TO DO (optional): Set an expiration date for sessions

		std::string cookieId = this->cookieSearch(sessions, reqHeaders);
		Cookie cookie;

		if (cookieId.size() <= 0)
		{
			this->_cookie = cookie.createCookie(path, method);
			this->_cookie._expirationDate = std::time(NULL) + COOKIE_TIMEOUT;
			this->_resHeaders["Set-Cookie"] = "sessionId=" + this->_cookie._id + "; HttpOnly; Expires=" + AuxFunc::getGmtTime(this->_cookie._expirationDate);
			// TO DO: Here we would set the new Cookie header using the id from Cookie create Cookie
		}
		else
		{
			this->_cookie = cookie.createCookie(path, method, cookieId);
			this->_cookie._expirationDate = sessions.find(cookieId)->second._expirationDate;
		}
	}

	std::string AServerAction::cookieSearch(
		const std::map<std::string, struct CookieData> &sessions,
		const std::map<std::string, std::string> &reqHeaders) const
	{
		std::map<std::string, std::string>::const_iterator cookieHeader;
		std::size_t cookieIndex;
		std::string cookieId;
		const std::string delimiter = "sessionId=";

		cookieHeader = reqHeaders.find("Cookie");
		if (cookieHeader == reqHeaders.end())
			return ("");
		cookieIndex = cookieHeader->second.find(delimiter);
		if (cookieIndex == std::string::npos)
			return ("");
		cookieId = cookieHeader->second.substr(cookieIndex + delimiter.size());
		if (sessions.find(cookieId) == sessions.end())
			return ("");
		return (cookieId);
	}

	void AServerAction::setContentType(const std::string &mime)
	{
		this->_resHeaders["Content-Type"] = mime;
	}

	void AServerAction::setContentLength(long size)
	{
		this->_resHeaders["Content-Length"] = AuxFunc::ft_itoa((unsigned int)size);
	}

	const std::string &AServerAction::getPath(void) const
	{
		return (this->_path);
	}

	const std::string &AServerAction::getContent(void) const
	{
		return (this->_content);
	}

	long AServerAction::getSize(void) const
	{
		return (this->_size);
	}

	unsigned int AServerAction::getResCode(void) const
	{
		return (this->_resCode);
	}

	const std::map<std::string, std::string> &AServerAction::getResHeaders(void) const
	{
		return (this->_resHeaders);
	}

	std::string AServerAction::getMime(void) const
	{
		std::map<std::string, std::string>::const_iterator it = this->_resHeaders.find("Content-Type");
		return it == this->_resHeaders.end() ? "" : it->second;
	}

	std::string AServerAction::getLocation(void) const
	{
		std::map<std::string, std::string>::const_iterator it = this->_resHeaders.find("Location");
		return it == this->_resHeaders.end() ? "" : it->second;
	}

	std::string AServerAction::getAllow(void) const
	{
		std::map<std::string, std::string>::const_iterator it = this->_resHeaders.find("Allow");
		return it == this->_resHeaders.end() ? "" : it->second;
	}

	void AServerAction::setRescode(unsigned int resCode)
	{
		this->_resCode = resCode;
	}

	AServerAction::HttpException::HttpException(unsigned int resCode)
	{
		this->_resCode = resCode;
	}

	unsigned int AServerAction::HttpException::getResCode(void) const
	{
		return (this->_resCode);
	}

	const char *AServerAction::HttpException::what(void) const throw()
	{
		return ("An http error has occurred");
	}

	AServerAction::~AServerAction() {}
}