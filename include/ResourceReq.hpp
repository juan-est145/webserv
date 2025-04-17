#ifndef RESOURCEREQ_HPP
#define RESOURCEREQ_HPP

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

namespace Webserv
{
	class ResourceReq
	{
	public:
		enum E_ResourceType
		{
			REG_FILE,
			DIRECTORY,
			CGI,
		};

		ResourceReq();
		ResourceReq(const ResourceReq &copy);
		ResourceReq &operator=(const ResourceReq &assign);

		int obtainResource(const std::string &uri);

		std::string getPath(void) const;
		const std::string &getContent(void) const;
		void setContent(const std::string &_content);
		long getSize(void) const;
		~ResourceReq();

	private:
		std::string _path;
		std::string _content;
		long _size;
		enum E_ResourceType _resourceType;

		int mapUriToResource(const std::string &uri);
		void readResource(void);
	};
}

#endif