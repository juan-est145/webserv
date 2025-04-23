/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostUpload.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:50:49 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/23 22:31:34 by juestrel         ###   ########.fr       */
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
	}

	PostUpload::PostUpload(std::string body, const std::string path) : AServerAction(path)
	{
		this->_body = body;
		this->_contentType = "";
		this->_contentLength = 0;
		this->_accept = "";
	}

	PostUpload::PostUpload(const PostUpload &copy): AServerAction(copy)
	{
		*this = copy;
	}

	PostUpload &PostUpload::operator=(const PostUpload &assign)
	{
		if (this != &assign)
		{
			this->_body = assign._body;
			this->_contentType = assign._contentType;
			this->_contentLength = assign._contentLength;
			this->_accept = assign._accept;
			this->_resCode = assign._resCode;
			this->_size = assign._resCode;
		}
		return *this;
	}

	void PostUpload::processRequest(const ConfigServer *config, const Request &req)
	{
		try
		{
			this->findHeaders(req);
			if (this->_contentType.substr(0, strlen("multipart/form-data;")) != "multipart/form-data;")
			{
				this->_resCode = 405;
				throw Webserv::AServerAction::HttpException();
			}
			this->uploadFile();
		}
		catch (const Webserv::AServerAction::HttpException &e)
		{
			this->processHttpError(config);
		}
	}

	void PostUpload::findHeaders(const Request &req)
	{
		std::map<std::string, std::string>::const_iterator it[3];
		const std::map<std::string, std::string> reqHeaders = req.getReqHeader();
		it[0] = req.getReqHeader().find("Content-Type");
		it[1] = req.getReqHeader().find("Content-Length");
		it[2] = req.getReqHeader().find("Accept");
		for (unsigned int i = 0; i < 3; i++)
			this->checkValidHeader(it[i], reqHeaders);
		this->_contentType = it[0]->second;
		this->_contentLength = std::atol(it[1]->second.c_str());
		this->_accept = it[2]->second;
	}

	void PostUpload::checkValidHeader(std::map<std::string, std::string>::const_iterator &it, const std::map<std::string, std::string> &reqHeaders)
	{
		if (it == reqHeaders.end())
		{
			this->_resCode = 400;
			throw Webserv::AServerAction::HttpException();
		}
	}

	void PostUpload::uploadFile(void)
	{
		std::string delimiter = this->obtainDelimiter();
		this->processUpload(delimiter);
	}

	std::string PostUpload::obtainDelimiter(void)
	{
		std::string delimiter = "; boundary=";
		std::size_t pos = this->_contentType.find(delimiter);
		if (pos == std::string::npos)
			throw Webserv::PostUpload::BodyParseError();
		return (this->_contentType.substr(pos + delimiter.length()));
	}

	void PostUpload::processUpload(std::string &boundary)
	{
		std::size_t startBound;
		std::size_t endBound;
		std::string delimiter = "\r\n";
		std::string file;
		std::size_t begginingIndex;
		std::map<std::string, std::string> metadata;

		startBound = this->_body.find("--" + boundary);
		endBound = this->_body.find("--" + boundary, 1);
		begginingIndex = boundary.length() + delimiter.length() + 2;
		file = this->_body.substr(begginingIndex, endBound - begginingIndex);
		this->_body.erase(0, file.size() + boundary.length() + delimiter.length() + 2);
		// TO DO: Test if we throw exception when we upload multiple files at the same time
		if (startBound == endBound || startBound == std::string::npos || endBound == std::string::npos)
			throw Webserv::PostUpload::BodyParseError();
		this->extractMetadata(metadata, file);
		this->downloadFile(metadata, file);
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
				throw Webserv::PostUpload::BodyParseError();
			headers[body.substr(0, separatorPos)] = body.substr(separatorPos + 2, newLinePos - separatorPos);
			body.erase(0, newLinePos + delimiter.length());
		}
		body.erase(0, 2);
	}

	void PostUpload::downloadFile(std::map<std::string, std::string> &headers, std::string &body)
	{
		std::size_t delimiterPos;
		std::string delimiter = "filename=";
		std::string fileNameField;
		std::string newLine = "\r\n";
		std::size_t newLinePos;

		delimiterPos = headers["Content-Disposition"].find(delimiter);
		newLinePos = headers["Content-Disposition"].find(newLine);
		if (delimiterPos == std::string::npos || newLinePos == std::string::npos)
			throw Webserv::PostUpload::BodyParseError();
		fileNameField = headers["Content-Disposition"].substr(delimiterPos + delimiter.length() + 1, newLinePos - (delimiterPos + delimiter.length()));
		if (fileNameField.find(";") != std::string::npos)
		{
			// TO DO: Implement something here
		}
		std::ofstream document(fileNameField.substr(0, fileNameField.length() - 2).c_str(), std::ios::out);
		if (!document.is_open())
			throw Webserv::PostUpload::UploadError();
		document << body.substr(0, body.length() - 2);
		document.close();
	}

	const char *PostUpload::BodyParseError::what(void) const throw()
	{
		return ("Invalid body found in this POST request");
	}

	const char *PostUpload::UploadError::what(void) const throw()
	{
		return ("Could not upload file to the server");
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
