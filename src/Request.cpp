/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:41 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/07 16:54:50 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"

namespace Webserv
{
	Request::Request(void)
	{
		this->_firstHeader.httpVers = "";
		this->_firstHeader.method = std::make_pair("", UNKNOWN);
		this->_firstHeader.path = "";
		this->_firstHeader.httpVers = "";
		this->_firstHeader.query = "";
		this->_socketFd = -1;
		this->_reqBody = "";
		this->_configuration = NULL;
		this->_serverAction = NULL;
	}

	Request::Request(int socketFd)
	{
		this->_firstHeader.httpVers = "";
		this->_firstHeader.method = std::make_pair("", UNKNOWN);
		this->_firstHeader.path = "";
		this->_firstHeader.httpVers = "";
		this->_firstHeader.query = "";
		this->_socketFd = socketFd;
		this->_reqBody = "";
		this->_configuration = NULL;
		this->_serverAction = NULL;
	}

	Request::Request(const Request &copy)
	{
		*this = copy;
	}

	Request &Request::operator=(const Request &assign)
	{
		if (this != &assign)
		{
			this->_reqHeader = assign._reqHeader;
			this->_reqBody = assign._reqBody;
			this->_firstHeader = assign._firstHeader;
			this->_serverAction = assign._serverAction;
			this->_configuration = assign._configuration;
		}
		return *this;
	}

	void Request::readReq(const char *buffer, size_t bufSize)
	{
		std::string strBuff(buffer, bufSize);

		this->extractHeaders(strBuff);
		this->_reqBody = strBuff;
	}

	void Request::handleReq(const std::vector<ConfigServer> &configs)
	{
		this->selectConfiguration(configs);
		this->getMethod().second == POST ? this->_serverAction = new Webserv::PostUpload(this->_reqBody, this->getPath()) : this->_serverAction = new Webserv::ResourceReq(this->getPath());
		this->_serverAction->processRequest(this->_configuration, *this);
	}

	void Request::extractHeaders(std::string &buffer)
	{
		this->extractFirstHead(buffer);
		std::string deli = "\r\n";
		std::queue<std::string> headers;
		std::size_t pos = buffer.find(deli);

		// TO DO: Set a 400 response for GET requests with a body.
		try
		{
			while (pos != std::string::npos && buffer.substr(0, 2) != "\r\n")
			{
				if (buffer.substr(0, pos).size() > 0)
					headers.push(buffer.substr(0, pos));
				buffer.erase(0, pos + deli.length());
				this->extractReqHead(headers);
				pos = buffer.find(deli);
			}
		}
		catch (const std::out_of_range &e)
		{
			// TO DO: Properly implement this
			//this->_resCode = 400;
			buffer = "";
			return;
		}
		buffer = buffer.substr(2);
	}

	void Request::extractReqHead(std::queue<std::string> &headers)
	{
		std::size_t pos;

		if (headers.size() <= 0)
			return;
		std::string temp = headers.front();
		headers.pop();
		pos = temp.find(":");
		if (pos == std::string::npos)
			exit(EXIT_FAILURE); // TO DO: Temp solution
		if (this->_reqHeader.find(temp.substr(0, pos)) != this->_reqHeader.end())
		{
			std::cout << "Repeated header, handle later" << std::endl;
			exit(EXIT_FAILURE);
		}
		this->_reqHeader[temp.substr(0, pos)] = temp.substr(pos + 2);
	}

	void Request::extractFirstHead(std::string &reqHeader)
	{
		std::string deli = "\r\n";
		std::size_t pos = reqHeader.find(deli);
		std::string firstLine;
		std::string temp;
		if (pos == std::string::npos) // TO DO: Temporary measure. Later on we should use this to set an appropiate response code.
			exit(EXIT_FAILURE);
		if (reqHeader.substr(0, pos).size() > 0)
			firstLine = reqHeader.substr(0, pos);
		reqHeader.erase(0, pos + deli.length());
		deli = " ";
		for (int i = 0; i < 3; i++)
		{
			pos = firstLine.find(deli);
			if (pos == std::string::npos && i != 2) // TO DO: Temporary measure. Later on we should use this to set an appropiate response code.
				exit(EXIT_FAILURE);
			if (firstLine.substr(0, pos).size() > 0)
				temp = firstLine.substr(0, pos);
			firstLine.erase(0, pos + deli.length());
			switch (i)
			{
			case 0:
				this->_firstHeader.method = this->selectMethod(temp);
				break;
			case 1:
				this->extractUrlAndQuery(temp);
				break;
			case 2:
				this->_firstHeader.httpVers = temp;
				break;
			default:
				break;
			}
		}
	}

	std::pair<std::string, enum method> Request::selectMethod(std::string &method)
	{
		std::string methods[3] = {
			"GET",
			"POST",
			"DELETE"};
		for (int i = 0; i < 3; i++)
		{
			if (methods[i] == method)
				return (std::make_pair(methods[i], (enum method)i));
		}
		return (std::make_pair("", UNKNOWN));
	}

	void Request::selectConfiguration(const std::vector<ConfigServer> &configs)
	{
		std::vector<ConfigServer>::const_iterator configuration = configs.begin();
		for (std::vector<ConfigServer>::const_iterator it = configs.begin(); it != configs.end(); it++)
		{
			std::string hostConfig = it->getPort() == 80 ? it->getServerName() : it->getServerName() + ":" + AuxFunc::ft_itoa(it->getPort());
			std::map<std::string, std::string>::iterator hostRequest = this->_reqHeader.find("Host");
			if (hostRequest != this->_reqHeader.end() && hostRequest->second == hostConfig)
			{
				configuration = it;
				break;
			}
		}
		this->_configuration = new ConfigServer(*configuration);
	}

	void Request::extractUrlAndQuery(const std::string &path)
	{
		size_t queryIndex = path.rfind("?");
		if (queryIndex == std::string::npos)
		{
			this->_firstHeader.path = path;
			return;
		}
		this->_firstHeader.path = path.substr(0, queryIndex);
		this->_firstHeader.query = path.substr(queryIndex + 1);
	}

	const std::map<std::string, std::string> &Request::getReqHeader(void) const
	{
		return (this->_reqHeader);
	}

	std::pair<std::string, enum method> Request::getMethod(void) const
	{
		return (this->_firstHeader.method);
	}

	const std::string &Request::getPath(void) const
	{
		return (this->_firstHeader.path);
	}

	const std::string &Request::getHttpVers(void) const
	{
		return (this->_firstHeader.httpVers);
	}

	unsigned int Request::getResCode(void) const
	{
		if (this->_serverAction == NULL)
			throw Request::RequestException();
		return (this->_serverAction->getResCode());
	}

	long Request::getResourceSize(void) const
	{
		if (this->_serverAction == NULL)
			throw Request::RequestException();
		return (this->_serverAction->getSize());
	}

	const std::string &Request::getReqBody(void) const
	{
		return (this->_reqBody);
	}

	const std::string &Request::getResourceContent(void) const
	{
		if (this->_serverAction == NULL)
			throw Request::RequestException();
		return (this->_serverAction->getContent());
	}

	const std::string &Request::getResourceMime(void) const
	{
		if (this->_serverAction == NULL)
			throw Request::RequestException();
		return (this->_serverAction->getMime());
	}

	const struct firstHeader &Request::getFirstHeader(void) const
	{
		return (this->_firstHeader);
	}

	void Request::setResCode(unsigned int resCode)
	{
		if (resCode < 100 || resCode > 511 || this->_serverAction == NULL)
			throw Webserv::Request::RequestException();
		this->_serverAction->setRescode(resCode);
	}

	std::size_t Request::setReqBody(std::string &body)
	{
		this->_reqBody.length() == 0 ? this->_reqBody = body : this->_reqBody.append(body);
		return (this->_reqBody.length());
	}

	const char *Request::RequestException::what(void) const throw()
	{
		return ("An invalid value was set to private member _resCode in Request class");
	}

	Request::~Request()
	{
		if (this->_configuration != NULL)
			delete this->_configuration;
		this->_configuration = NULL;
		if (this->_serverAction != NULL)
			delete this->_serverAction;
		this->_serverAction = NULL;
	}
}