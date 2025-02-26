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

		startBound = this->_body.find("--" + boundary);
		endBound = this->_body.find("--" + boundary, 1);
		begginingIndex = boundary.length() + delimiter.length() + 2;
		file = this->_body.substr(begginingIndex, endBound - begginingIndex);
		this->_body.erase(0, file.size() + boundary.length() + delimiter.length() + 2);
		// TO DO: Implement better error handling
		if (startBound == endBound || startBound == std::string::npos || endBound == std::string::npos)
			exit(EXIT_FAILURE);
		
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
