#include "ResourceReq.hpp"

namespace Webserv
{
	// TO DO: Later on we need a public function that takes as an argument the type of verb of the request
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

	int ResourceReq::obtainResource(const std::string &uri)
	{
		struct stat fileStat;
		int responseCode;

		// TO DO: Handle somehow 500 error codes later on
		responseCode = this->mapUriToResource(uri);
		if (access(this->_path.c_str(), R_OK) == -1)
			responseCode = 500;
		// TO DO: Implement exception here if stat fails
		stat(this->_path.c_str(), &fileStat);
		this->_size = fileStat.st_size;
		this->readResource();
		return (responseCode);
	}

	int ResourceReq::mapUriToResource(const std::string &uri)
	{
		// TO DO: Once we have a working configuration file, we should use that information instead
		int responseCode = 200;

		if (uri == "/")
			this->_path = "./html/index.html";
		else if (uri == "/upload")
			this->_path = "./html/upload.html";
		else
		{
			this->_path = "./html/error404.html";
			responseCode = 404;
		}
		return (responseCode);
	}

	void ResourceReq::readResource(void)
	{
		std::ifstream resource;
		char *buffer = NULL;

		resource.open(this->_path, std::ios::in);
		buffer = new char[this->_size  + 1];
		// TO DO: Check that resource is open and throw exception if it is not
		resource.read(buffer, this->_size);
		this->_content = buffer;
		delete[] buffer;
	}

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

	ResourceReq::~ResourceReq() {}
}
