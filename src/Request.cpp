/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:41 by juestrel          #+#    #+#             */
/*   Updated: 2025/02/21 11:10:34 by juestrel         ###   ########.fr       */
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
		this->_socketFd = -1;
		this->_reqBody = "";
	}

	Request::Request(int socketFd)
	{
		this->_method = UNKNOWN;
		this->_path = "";
		this->_httpVers = "";
		this->_resCode = 200;
		this->_socketFd = socketFd;
		this->_reqBody = "";
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
		std::string strBuff(buffer);

		this->extractHeaders(strBuff);
		this->_reqBody = strBuff;
		if (this->_method == POST)
		{
			// TO DO: Make sure to later validate that the fields being passed exist
			PostUpload upload(this->_reqHeader["Content-Type"], std::atol(this->_reqHeader["Content-Length"].c_str()), this->_reqHeader["Accept"]);
			upload.uploadFile();
		}
		this->_resCode = this->_resourceReq.obtainResource(this->_path);
	}

	void Request::extractHeaders(std::string &buffer)
	{
		this->extractFirstHead(buffer);
		std::string deli = "\r\n";
		std::queue<std::string> headers;
		std::size_t pos = buffer.find(deli);

		// TO DO: Implement body extraction for POST and DELETE requests. Might need to change the extraction
		// algorithm. Also set a 400 response for GET requests with a body.
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
			this->_resCode = 400;
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

	long Request::getResourceSize(void) const
	{
		return (this->_resourceReq.getSize());
	}

	const std::string &Request::getResourceContent(void) const
	{
		return (this->_resourceReq.getContent());
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