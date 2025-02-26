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
		std::string uploadContent;
		bool boundaryFound = false;
		std::string delimiter = "\r\n";
		std::size_t deliPos = this->_body.find(delimiter);

		this->_body.substr(0, deliPos) == "--" + boundary ? std::cout << "Todo bien" : std::cout << "Eres subnormal Juan xd";
		this->_body.erase(0, deliPos + 2);
		(void)uploadContent;
		(void)boundaryFound;
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
