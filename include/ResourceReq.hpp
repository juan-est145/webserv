#ifndef RESOURCEREQ_HPP
#define RESOURCEREQ_HPP

#include <iostream>
#include <string>

namespace Webserv
{
	class ResourceReq
	{
	public:
		enum E_ResourceType
		{
			REG_FILE,
			BIN_FILE,
			DIRECTORY,
			CGI,
		};
		ResourceReq();
		ResourceReq(const ResourceReq &copy);
		ResourceReq &operator=(const ResourceReq &assign);
		std::string getPath(void) const;
		std::string getContent(void) const;
		void setContent(const std::string &_content);
		long getSize(void) const;
		~ResourceReq();

	private:
		enum E_ResourceType;
		std::string _path;
		std::string _content;
		long _size;
		enum E_ResourceType _resourceType;
	};
}

#endif