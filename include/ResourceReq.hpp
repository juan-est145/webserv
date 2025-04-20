#ifndef RESOURCEREQ_HPP
#define RESOURCEREQ_HPP

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include "AServerAction.hpp"

namespace Webserv
{
	class ResourceReq: public AServerAction
	{
	public:
		enum E_ResourceType
		{
			REG_FILE,
			DIRECTORY,
			CGI,
		};

		ResourceReq(void);
		ResourceReq(const std::string path);
		ResourceReq(const ResourceReq &copy);
		ResourceReq &operator=(const ResourceReq &assign);

		void processRequest(ConfigServer *config);

		void setContent(const std::string &_content);
		~ResourceReq();

	private:
		enum E_ResourceType _resourceType;

		int mapUriToResource(const std::string &uri);
		void readResource(void);
	};
}

#endif