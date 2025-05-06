/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:13:04 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/06 12:07:50 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cgi.hpp"

namespace Webserv
{
	Cgi::Cgi() : _locationConf(NULL)
	{
		this->_interpreter = "";
		this->_pathInfo = "";
	}

	Cgi::Cgi(const Location &location) : _locationConf(&location)
	{
		this->_interpreter = "";
		this->_pathInfo = "";
	}

	Cgi::Cgi(const Cgi &toCopy) : _locationConf(toCopy._locationConf)
	{
		*this = toCopy;
	}

	Cgi &Cgi::operator=(const Cgi &toCopy)
	{
		if (this != &toCopy)
		{
			this->_interpreter = toCopy._interpreter;
			this->_locationConf = toCopy._locationConf;
			this->_pathInfo = toCopy._pathInfo;
		}
		return (*this);
	}

	bool Cgi::canProcessAsCgi(const std::string &path, const std::map<std::string, std::string> &headers)
	{
		std::vector<std::string> segmentedPath;
		const std::string delimiter = "/";
		std::string copy(path);
		size_t pos = 0;

		while ((pos = copy.find(delimiter)) != std::string::npos)
		{
			segmentedPath.push_back(copy.substr(0, pos));
			copy.erase(0, pos + delimiter.size());
		}
		if (copy.size() > 0)
			segmentedPath.push_back(copy);
		const std::pair<cgiExtenIndex, urlSegmentIndex> indexes = this->selectCgiExtensions(segmentedPath);
		if (indexes.first == -1 || indexes.second == -1)
			return (false);
		this->extractPathInfoAndInter(indexes, path, segmentedPath);
		this->execCgi(this->findCgiFile(path, segmentedPath, indexes), headers);
		return (true);
	}

	std::pair<Cgi::cgiExtenIndex, Cgi::urlSegmentIndex> Cgi::selectCgiExtensions(const std::vector<std::string> &segmentedPath) const
	{
		std::pair<cgiExtenIndex, urlSegmentIndex> indexes = std::make_pair(-1, -1);
		const std::vector<std::string> cgiExten = this->_locationConf->getCgiExtension();

		for (size_t i = 0; i < segmentedPath.size(); i++)
		{
			for (size_t extenIndex = 0; extenIndex < cgiExten.size(); extenIndex++)
			{
				size_t startIndex = segmentedPath[i].rfind(cgiExten[extenIndex]);
				if (startIndex != std::string::npos && segmentedPath[i].substr(startIndex) == cgiExten[extenIndex])
				{
					indexes.first = extenIndex;
					indexes.second = i;
					return (indexes);
				}
			}
		}
		return (indexes);
	}

	void Cgi::extractPathInfoAndInter(
		const std::pair<cgiExtenIndex, urlSegmentIndex> &indexes,
		const std::string &path,
		const std::vector<std::string> &segmentedPath)
	{
		this->_interpreter = this->_locationConf->getCgiPath()[indexes.first];
		size_t pathInfoIndex = path.find(segmentedPath[indexes.second]) + segmentedPath[indexes.second].size();
		this->_pathInfo = pathInfoIndex < path.size() ? path.substr(pathInfoIndex) : "";
	}

	std::string Cgi::findCgiFile(
		const std::string &path,
		const std::vector<std::string> segmentedPath,
		const std::pair<Cgi::cgiExtenIndex, Cgi::urlSegmentIndex> &indexes)
	{
		std::string cgiUrl = path.substr(0, this->_pathInfo.size() <= 0 ? path.size() : path.find(segmentedPath[indexes.second]) + segmentedPath[indexes.second].size());
		std::string localPath = AuxFunc::mapPathToResource(*this->_locationConf, cgiUrl);

		if (access(localPath.c_str(), F_OK) == -1)
		{
			std::cout << "The file does not exist in cgi directory lol" << std::endl;
			// TO DO: Throw an exception that later marks it as a 404 error in ResourceReq or PostUpload. Also delete the std::cout
		}
		if (access(localPath.c_str(), X_OK) == -1)
		{
			std::cout << "We can't execute the script in cgi directory lol" << std::endl;
			// TO DO: Throw an exception that later marks it as a 500 error in ResourceReq or PostUpload. Also delete the std::cout
		}
		return (localPath);
	}

	void Cgi::execCgi(const std::string &localPath, const std::map<std::string, std::string> &headers) const
	{
		(void)localPath;
		(void)headers;
		int pipeFd[2];
		if (pipe(pipeFd) == -1)
		{
			std::cout << "Pipe got fucked xd" << std::endl;
			// TO DO: Throw an appropiate exeception that caller class must transform into http error code
			// Also delete the cout
		}
		pid_t pid = fork();
		if (pid == -1)
		{
			close(pipeFd[PIPE_READ]);
			close(pipeFd[PIPE_WRITE]);
			// TO DO: Throw an appropiate exeception that caller class must transform into http error code
			// Also delete the cout
		}
		else if (pid == 0)
			exit(EXIT_SUCCESS); // Temporary, here we would call the execute function for child process
		if (close(pipeFd[PIPE_WRITE]) == -1)
		{
			close(pipeFd[PIPE_READ]);
			// TO DO: Throw an appropiate exeception that caller class must transform into http error code
		}
	}

	Cgi::~Cgi() {}
}