#include "ResourceReq.hpp"

namespace Webserv
{
	ResourceReq::ResourceReq()
	{
		this->_path = "";
		this->_content = "";
		this->_size = 0;
	}

	ResourceReq::ResourceReq(const ResourceReq &copy)
	{
		*this = copy;
	}

	ResourceReq &ResourceReq::operator=(const ResourceReq &assign)
	{
		if (this != &assign)
		{
			this->_path = assign._path;
			this->_content = assign._content;
			this->_size = assign._size;
		}
		return *this;
	}

	ResourceReq::~ResourceReq() {}

	std::string ResourceReq::getPath(void) const
	{
		return this->_path;
	}
	std::string ResourceReq::getContent(void) const
	{
		return this->_content;
	}
	void ResourceReq::setContent(const std::string &_content)
	{
		this->_content = _content;
	}

	long ResourceReq::getSize() const
	{
		return this->_size;
	}
}
