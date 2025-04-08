#include "ConfigParser.hpp"

namespace Webserv
{
	ConfigParser::ConfigParser()
	{
		this->_nServers = 0;
	}

	int ConfigParser::initConfigParser(const std::string &config_file)
	{
		std::string content;
		ConfigFile file(config_file);

		if (file.getPathType(file.getPath()) != ConfigFile::PATH_FILE)
			throw ErrorException("File is invalid");
		if (file.fileOk(file.getPath()) == -1)
			throw ErrorException(("File can't be accessed"));
		content = file.readFile(config_file);
		if (content.empty())
			throw ErrorException("File is empty");
		this->removeComments(content);
		this->removeSpaces(content);
		this->initServersConfig(content);
		if (this->_serversConfig.size() != this->_nServers)
			throw ErrorException("Sizes don't match");
		for (size_t i = 0; i < this->_nServers; i++)
		{
			ConfigServer server;

			this->createServer(this->_serversConfig[i], server);
			this->_servers.push_back(server);
		}
		return (0);
	}

	void ConfigParser::removeComments(std::string &content)
	{
		size_t pos;
		size_t pos_end;

		pos = content.find('#');
		while (pos != std::string::npos)
		{
			pos_end = content.find('\n', pos);
			content.erase(pos, pos_end - pos);
			pos = content.find('#');
		}
	}

	void ConfigParser::removeSpaces(std::string &content)
	{
		size_t i = 0;

		while (content[i] && isspace(content[i]))
			i++;
		content = content.substr(i);
		i = content.length() - 1;
		while (i > 0 && isspace(content[i]))
			i--;
		content = content.substr(0, i + 1);
	}

	void ConfigParser::initServersConfig(std::string &content)
	{
		size_t start = 0;
		size_t end = 1;

		if (content.find("server", 0) == std::string::npos)
			throw ErrorException("Server not found");
		while (start != end && start < content.length())
		{
			start = findStartServer(start, content);
			end = findEndServer(start, content);
			if (start == end)
				throw ErrorException("Server is size 0");
			this->_serversConfig.push_back(content.substr(start, end - start + 1));
			this->_nServers++;
			start = end + 1;
		}
	}

	size_t ConfigParser::findStartServer(size_t start, std::string &content)
	{
		size_t i;

		i = start;
		while (content[i] && (content[i] != 's'))
		{
			if (!isspace(content[i]))
				throw ErrorException("Wrong character out of server {}");
			i++;
		}
		if (!content[i])
			return (start);
		if (content.compare(i, 6, "server") != 0)
			throw ErrorException("Wrong character out of server {}");
		i += 6;
		while (content[i] && isspace(content[i]))
			i++;
		if (content[i] != '{')
			throw ErrorException("Wrong character out of server {}");
		return (i);
	}

	size_t ConfigParser::findEndServer(size_t start, std::string &content)
	{
		size_t i;
		size_t scope;

		i = start + 1;
		scope = 0;
		while (content[i])
		{
			if (content[i] == '{')
				scope++;
			else if ((content[i] == '}') && (scope == 0))
				return (i);
			else if (content[i] == '}')
				scope--;
		}
		return (start);
	}

	static std::vector<std::string> splitParameters(std::string line, std::string del)
	{
		std::vector<std::string> strs;
		std::string temp;
		size_t start, end;

		start = 0;
		end = 0;
		while (1)
		{
			end = line.find_first_of(del, start);
			if (end == std::string::npos)
				return (strs);
			temp = line.substr(start, end - start);
			strs.push_back(temp);
			start = line.find_first_not_of(del, end);
			if (start == std::string::npos)
				return (strs);
		}
		return (strs);
	}

	int ConfigParser::strnCompare(std::string str1, std::string str2, size_t pos)
	{
		size_t i;

		i = 0;
		while (pos < str1.length() && i < str2.length() && str1[pos] == str2[pos])
		{
			pos++;
			i++;
		}
		if (i == str2.length() && pos <= str1.length() && (str1.length() == pos || isspace(str1[pos])))
			return (0);
		return (1);
	}

	void ConfigParser::createServer(std::string &config, ConfigServer &server)
	{
	}

	void ConfigParser::checkServers()
	{
		std::vector<ConfigServer>::iterator it1;
		std::vector<ConfigServer>::iterator it2;

		for (it1 = this->_servers.begin();
			 it1 != this->_servers.end() - 1; it1++)
		{
			for (it2 = it1 + 1;
				 it2 != this->_servers.end(); it2++)
			{
				if (it1->getPort() == it2->getPort() && it1->getHost() == it2->getHost() && it1->getServerName() == it2->getServerName())
					throw ErrorException("Failed server validation");
			}
		}
	}

	std::vector<ConfigServer> ConfigParser::getServers()
	{
		return (this->_servers);
	}

	int ConfigParser::print()
	{
	}

	ConfigParser::~ConfigParser() {}

}
