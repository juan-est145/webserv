#include "../include/PostUpload.hpp"

namespace Webserv
{
	PostUpload::PostUpload(void)
	{
		this->_contentType = "";
		this->_contentLength = 0;
		this->_accept = "";
	}

	PostUpload::PostUpload(std::string contentType, long contentLength, std::string accept)
	{
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
			this->_contentType = assign._contentType;
			this->_contentLength = assign._contentLength;
			this->_accept = assign._accept;
		}
		return *this;
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
