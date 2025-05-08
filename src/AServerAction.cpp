/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerAction.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:05:50 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/08 19:27:08 by juestrel         ###   ########.fr       */
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
		this->_mime = "text/plain";
	}

	AServerAction::AServerAction(const std::string path) : _path(path)
	{
		this->_content = "";
		this->_size = -1;
		this->_resCode = 200;
		this->_mime = "text/plain";
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
			this->_mime = toCopy._mime;
		}
		return (*this);
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
		this->readResource(localPath);
	}

	void AServerAction::readResource(const std::string &path)
	{
		std::ifstream resource;
		char *buffer = NULL;

		resource.open(path.c_str(), std::ios::in);
		if (!resource.is_open())
		{
			this->_resCode = 500;
			throw Webserv::AServerAction::HttpException();
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

	void AServerAction::isMethodAllowed(const Location &locationFile, const std::string &method)
	{
		std::map<std::string, bool>::const_iterator it = locationFile.getMethods().find(method);
		if (it == locationFile.getMethods().end() || !it->second)
		{
			this->_resCode = 405;
			throw Webserv::AServerAction::HttpException();
		}
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
				this->_mime = "text/html";
				return (true);
			}
		}
		catch (const Webserv::Cgi::NotFoundException &e)
		{
			this->_resCode = 404;
			throw Webserv::AServerAction::HttpException();
		}
		catch (const Webserv::Cgi::CgiErrorException &e)
		{
			this->_resCode = 500;
			throw Webserv::AServerAction::HttpException();
		}
		return (false);
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

	const std::string &AServerAction::getMime(void) const
	{
		return (this->_mime);
	}

	void AServerAction::setRescode(unsigned int resCode)
	{
		this->_resCode = resCode;
	}

	const char *AServerAction::HttpException::what(void) const throw()
	{
		return ("An http error has occurred");
	}

	AServerAction::~AServerAction() {}
}