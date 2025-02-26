#include "../include/PostUpload.hpp"

namespace Webserv
{
	PostUpload::PostUpload(void)
	{
		this->_body = "";
		this->_contentType = "";
		this->_contentLength = 0;
		this->_accept = "";
	}

	PostUpload::PostUpload(std::string body, std::string contentType, long contentLength, std::string accept)
	{
		this->_body = body;
		this->_contentType = contentType;
		this->_contentLength = contentLength;
		this->_accept = accept;
	}

	PostUpload::PostUpload(const PostUpload &copy)
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
		}
		return *this;
	}

	void PostUpload::uploadFile(void)
	{
		std::string delimiter = this->obtainDelimiter();
		this->processUpload(delimiter);
	}

	std::string PostUpload::obtainDelimiter(void)
	{
		// TO DO: If pos is npos handle accordingly
		std::string delimiter = "; boundary=";
		std::size_t pos = this->_contentType.find(delimiter);
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
		// TO DO: Implement better error handling
		if (startBound == endBound || startBound == std::string::npos || endBound == std::string::npos)
			exit(EXIT_FAILURE);
		this->extractMetadata(metadata, file);
		this->downloadFile(metadata, file);
	}

	void PostUpload::extractMetadata(std::map<std::string, std::string> &headers, std::string &body)
	{
		std::string delimiter = "\r\n";
		std::size_t newLinePos;
		std::size_t separatorPos;

		// TO DO: Check that we don't get npos in the separators pos
		while (body.substr(0, 2) != delimiter)
		{
			newLinePos = body.find(delimiter);
			separatorPos = body.find(":");
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
		
		// TO DO: Check the value of delimiterPos
		delimiterPos = headers["Content-Disposition"].find(delimiter);
		newLinePos = headers["Content-Disposition"].find(newLine);
		fileNameField = headers["Content-Disposition"].substr(delimiterPos + delimiter.length() + 1, newLinePos - (delimiterPos + delimiter.length()));
		if (fileNameField.find(";") != std::string::npos)
		{
			// TO DO: Implement something here
		}
		std::ofstream document(fileNameField.substr(0, fileNameField.length() - 2).c_str(), std::ios::out);
		if (document.is_open())
		{
			document << body.substr(0, body.length() - 2);
			document.close();
		}
		else
		{
			// TO DO: Implement this correctly
			exit(EXIT_FAILURE);
		}
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
