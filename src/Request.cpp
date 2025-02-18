/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:41 by juestrel          #+#    #+#             */
/*   Updated: 2025/02/18 17:47:28 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Request.hpp"

namespace Webserv
{
	Request::Request(void)
	{
		this->_method = UNKNOWN;
		this->_path = "";
		this->_httpVers = "";
		this->_resCode = 200;
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
			this->_method = assign._method;
			this->_httpVers = assign._httpVers;
			this->_path = assign._path;
			this->_resCode = assign._resCode;
			this->_resourceReq = assign._resourceReq;
		}
		return *this;
	}

	// TO DO: REMEMBER TO DECODE HTTP REQUEST HEADERS. IS A MUST
	void Request::processReq(const char *buffer)
	{
		
		this->extractHeaders(buffer);
		this->_resourceReq.obtainResource(this->_path);
		// TO DO: Maybe later we will need to use a wrapper for the struct in request
		// TO DO: Check the return value of fileStat
		stat(this->_resourceData.path.c_str(), &fileStat);
		this->_resourceData.size = fileStat.st_size;
		this->_resourceData.resourceType = REG_FILE;
	}

	void Request::extractHeaders(const char *buffer)
	{
		std::string reqHeader(buffer);
		this->extractFirstHead(reqHeader);
		std::string deli = "\r\n";
		std::queue<std::string> headers;
		std::size_t pos = reqHeader.find(deli);

		// TO DO: Implement body extraction for POST and DELETE requests. Might need to change the extraction
		// algorithm. Also set a 400 response for GET requests with a body.
		while (pos != std::string::npos)
		{
			if (reqHeader.substr(0, pos).size() > 0)
				headers.push(reqHeader.substr(0, pos));
			reqHeader.erase(0, pos + deli.length());
			this->extractReqHead(headers);
			pos = reqHeader.find(deli);
		}
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
				this->_method = this->selectMethod(temp);
				break;
			case 1:
				this->_path = temp;
				break;
			case 2:
				this->_httpVers = temp;
				break;
			default:
				break;
			}
		}
	}

	

	Request::E_Method Request::selectMethod(std::string &method)
	{
		std::string methods[3] = {
			"GET",
			"POST",
			"DELETE"};
		for (int i = 0; i < 3; i++)
		{
			if (methods[i] == method)
				return ((Request::E_Method)i);
		}
		return (UNKNOWN);
	}

	const std::map<std::string, std::string> &Request::getReqHeader(void) const
	{
		return (this->_reqHeader);
	}

	Request::E_Method Request::getMethod(void) const
	{
		return (this->_method);
	}

	const std::string &Request::getPath(void) const
	{
		return (this->_path);
	}

	const std::string &Request::getHttpVers(void) const
	{
		return (this->_httpVers);
	}

	unsigned int Request::getResCode(void) const
	{
		return (this->_resCode);
	}

	const struct Request::S_Resource &Request::getResourceData(void) const
	{
		return (this->_resourceData);
	}

	void Request::setResCode(unsigned int resCode)
	{
		if (resCode < 100 || resCode > 511)
			throw Webserv::Request::RequestException();
		this->_resCode = resCode;
	}

	const char *Request::RequestException::what(void) const throw()
	{
		return ("An invalid value was set to private member _resCode in Request class");
	}

	Request::~Request() {}
}