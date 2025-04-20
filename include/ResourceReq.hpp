#ifndef RESOURCEREQ_HPP
#define RESOURCEREQ_HPP

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <algorithm>
#include "AServerAction.hpp"
#include "ConfigServer.hpp"

namespace Webserv
{
	class ConfigServer;

	class ResourceReq : public AServerAction
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

		void processRequest(const ConfigServer *config);

		void setContent(const std::string &_content);
		~ResourceReq();

	private:
		enum E_ResourceType _resourceType;

		void obtainResource(const ConfigServer *config);
		const Location &obtainLocationConf(const ConfigServer *config) const;
		std::string mapPathToResource(const Location &locationFile) const;
		void readResource(const std::string &path);
	};
}

#endif