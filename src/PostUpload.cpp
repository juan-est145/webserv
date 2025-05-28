/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:50:49 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/28 20:52:33 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PostUpload.hpp"

namespace Webserv
{
	PostUpload::PostUpload(void) : AServerAction()
	{
		this->_body = "";
		this->_contentType = "";
		this->_contentLength = 0;
		this->_accept = "";
		this->_resCode = 201;
	}

	PostUpload::PostUpload(std::string body, const std::string path) : AServerAction(path)
	{
		this->_body = body;
		this->_contentType = "";
		this->_contentLength = 0;
		this->_accept = "";
		this->_resCode = 201;
	}

	PostUpload::PostUpload(const PostUpload &copy) : AServerAction(copy)
	{
		*this = copy;
	}

	PostUpload &PostUpload::operator=(const PostUpload &assign)
	{
		if (this != &assign)
		{
			this->_content = assign._content;
			this->_size = assign._size;
			this->_resCode = assign._resCode;
			this->_resHeaders = assign._resHeaders;
			this->_body = assign._body;
			this->_contentType = assign._contentType;
			this->_contentLength = assign._contentLength;
			this->_accept = assign._accept;
		}
		return (*this);
	}

	void PostUpload::processRequest(const ConfigServer *config, 
		const Request &req, 
		const std::map<std::string, Webserv::CookieData> &sessions)
	{
		try
		{
			this->handleCookies(req.getReqHeader(), req.getPath(), req.getMethod().first, sessions);
			const Location locationFile = this->obtainLocationConf(config);
			std::string localPath = AuxFunc::mapPathToResource(locationFile, this->_path);

			if (localPath[localPath.size() - 1] != '/')
				localPath += "/";
			this->isMethodAllowed(locationFile, req.getMethod().first);
			if (req.getHttpVers() != "HTTP/1.1")
				throw Webserv::AServerAction::HttpException(505);
			if (locationFile.getMaxBodySize() < req.getReqBody().size())
				throw Webserv::AServerAction::HttpException(413);
			this->findHeaders(req);
			if (locationFile.getReturn().size() > 0)
			{
				this->redirect(locationFile.getReturn(), config);
				return;
			}
			if (locationFile.getCgiPath().size() > 0)
			{
				if (this->isCgi(locationFile, req.getPath(), req.getReqHeader(), config, req.getFirstHeader(), req.getReqBody(), req))
					return;
			}
			if (access(localPath.c_str(), F_OK) == -1)
				throw Webserv::AServerAction::HttpException(404);
			else if (access(localPath.c_str(), W_OK) == -1)
				throw Webserv::AServerAction::HttpException(500);
			if (this->_contentType.substr(0, strlen("multipart/form-data;")) != "multipart/form-data;")
				throw Webserv::AServerAction::HttpException(405);
			this->uploadFile(localPath);
			this->createBodyMessage();
			this->addLocationMember(config);
		}
		catch (const Webserv::AServerAction::HttpException &e)
		{
			this->_resCode = e.getResCode();
			this->processHttpError(config);
			this->setContentType("text/html");
		}
	}

	void PostUpload::findHeaders(const Request &req)
	{
		std::map<std::string, std::string>::const_iterator it[3];
		const std::map<std::string, std::string> reqHeaders = req.getReqHeader();
		it[0] = reqHeaders.find("Content-Type");
		it[1] = reqHeaders.find("Accept");
		it[2] = reqHeaders.find("Content-Length");
		// We DO NOT want to use checkValidHeader on Content-Length
		for (unsigned int i = 0; i < (sizeof(it) / sizeof(it[0])) - 1; i++)
			this->checkValidHeader(it[i], reqHeaders);
		this->_contentType = it[0]->second;
		this->_accept = it[1]->second;
		this->_contentLength = it[2] == reqHeaders.end() ? req.getReqBody().size() : std::atol(it[2]->second.c_str());
	}

	void PostUpload::checkValidHeader(std::map<std::string, std::string>::const_iterator &it, const std::map<std::string, std::string> &reqHeaders)
	{
		if (it == reqHeaders.end())
			throw Webserv::AServerAction::HttpException(400);
	}

	void PostUpload::uploadFile(const std::string &localPath)
	{
		std::string delimiter = this->obtainDelimiter();
		this->processUpload(delimiter, localPath);
	}

	std::string PostUpload::obtainDelimiter(void)
	{
		std::string delimiter = "; boundary=";
		std::size_t pos = this->_contentType.find(delimiter);
		if (pos == std::string::npos)
			throw Webserv::AServerAction::HttpException(400);
		return (this->_contentType.substr(pos + delimiter.length()));
	}

	void PostUpload::processUpload(std::string &boundary, const std::string &localPath)
	{
		std::size_t startBound;
		std::size_t endBound;
		std::string delimiter = "\r\n";
		std::string file;
		std::size_t begginingIndex;
		std::map<std::string, std::string> metadata;

		while (this->_body != "--" + boundary + "--\r\n")
		{
			startBound = this->_body.find("--" + boundary);
			endBound = this->_body.find("--" + boundary, 1);
			begginingIndex = boundary.length() + delimiter.length() + 2;
			file = this->_body.substr(begginingIndex, endBound - begginingIndex);
			this->_body.erase(0, file.size() + boundary.length() + delimiter.length() + 2);
			if (startBound == endBound || startBound == std::string::npos || endBound == std::string::npos)
				throw Webserv::AServerAction::HttpException(400);
			this->extractMetadata(metadata, file);
			this->downloadFile(metadata, file, localPath);
		}
	}

	void PostUpload::extractMetadata(std::map<std::string, std::string> &headers, std::string &body)
	{
		std::string delimiter = "\r\n";
		std::size_t newLinePos;
		std::size_t separatorPos;

		while (body.substr(0, 2) != delimiter)
		{
			newLinePos = body.find(delimiter);
			separatorPos = body.find(":");
			if (newLinePos == std::string::npos || separatorPos == std::string::npos)
				throw Webserv::AServerAction::HttpException(400);
			headers[body.substr(0, separatorPos)] = body.substr(separatorPos + 2, newLinePos - separatorPos);
			body.erase(0, newLinePos + delimiter.length());
		}
		body.erase(0, 2);
	}

	void PostUpload::downloadFile(std::map<std::string, std::string> &headers, std::string &body, const std::string &localPath)
	{
		std::size_t delimiterPos;
		std::string delimiter = "filename=";
		std::string fileNameField;
		std::string newLine = "\r\n";
		std::size_t newLinePos;

		if (body == "\r\n" || body.size() == 0)
			return;
		delimiterPos = headers["Content-Disposition"].find(delimiter);
		newLinePos = headers["Content-Disposition"].find(newLine);
		if (delimiterPos == std::string::npos || newLinePos == std::string::npos)
			throw Webserv::AServerAction::HttpException(400);
		fileNameField = localPath + headers["Content-Disposition"].substr(delimiterPos + delimiter.length() + 1, newLinePos - (delimiterPos + delimiter.length()));
		if (fileNameField.find(";") != std::string::npos)
			throw Webserv::AServerAction::HttpException(400);
		std::ofstream document(fileNameField.substr(0, fileNameField.length() - 2).c_str(), std::ios::out);
		if (!document.is_open())
			throw Webserv::AServerAction::HttpException(500);
		document << body.substr(0, body.length() - 2);
		document.close();
	}

	void PostUpload::createBodyMessage(void)
	{
		this->_content = "Resource was uploaded correctly on path " + this->_path;
		this->_size = this->_content.length();
		this->setContentLength(this->_size);
		this->setContentType("text/plain");
	}

	void PostUpload::addLocationMember(const ConfigServer *config)
	{
		this->_resHeaders["Location"] = "http://" + config->getServerName();
		this->_resHeaders["Location"] += config->getPort() == 80 ? "" : ":" + AuxFunc::ft_itoa(config->getPort());
		this->_resHeaders["Location"] += this->_path;
	}

	void PostUpload::redirect(const std::string &uri, const ConfigServer *config)
	{
		this->_resCode = 308;
		this->setContentType("text/html");
		if (uri.substr(0, 7) == "http://" || uri.substr(0, 8) == "https://")
		{
			this->_resHeaders["Location"] = uri;
			return;
		}
		this->_resHeaders["Location"] = "http://" + config->getServerName();
		this->_resHeaders["Location"] += config->getPort() == 80 ? "" : ":" + AuxFunc::ft_itoa(config->getPort());
		this->_resHeaders["Location"] += uri;
	}

	const std::string &PostUpload::getContentType(void) const
	{
		return this->_contentType;
	}

	long PostUpload::getContentLength(void) const
	{
		return this->_contentLength;
	}

	const std::string &PostUpload::getAccept(void) const
	{
		return this->_accept;
	}

	PostUpload::~PostUpload() {}
}
