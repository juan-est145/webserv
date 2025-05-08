/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:13:04 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/08 18:59:27 by juestrel         ###   ########.fr       */
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

	bool Cgi::canProcessAsCgi(
		const std::string &path,
		const std::map<std::string, std::string> &headers,
		std::string &content,
		const ConfigServer *config,
		const struct firstHeader &firstHeader,
		const std::string &body)
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
		this->execCgi(path, this->findCgiFile(path, segmentedPath, indexes), headers, content, config, firstHeader, body);
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
		const std::pair<Cgi::cgiExtenIndex, Cgi::urlSegmentIndex> &indexes) const
	{
		std::string cgiUrl = path.substr(0, this->_pathInfo.size() <= 0 ? path.size() : path.find(segmentedPath[indexes.second]) + segmentedPath[indexes.second].size());
		std::string localPath = AuxFunc::mapPathToResource(*this->_locationConf, cgiUrl);

		if (access(localPath.c_str(), F_OK) == -1)
			throw Webserv::Cgi::NotFoundException();
		if (access(localPath.c_str(), X_OK) == -1)
			throw Webserv::Cgi::CgiErrorException();
		return (localPath);
	}

	void Cgi::execCgi(
		const std::string &path,
		const std::string &localPath,
		const std::map<std::string, std::string> &headers,
		std::string &content,
		const ConfigServer *config,
		const struct firstHeader &firstHeader,
		const std::string &body) const
	{
		int pipeFd[2];
		char buffer[1024];
		int status = 0;

		memset(buffer, '\0', sizeof(buffer));
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
			this->childProcess(pipeFd, path, localPath, headers, config, firstHeader);
		// TO DO: Check the value of write
		write(pipeFd[PIPE_WRITE], body.c_str(), body.size());
		waitpid(pid, &status, 0);
		if (close(pipeFd[PIPE_WRITE]) == -1)
		{
			close(pipeFd[PIPE_READ]);
			// TO DO: Throw an appropiate exeception that caller class must transform into http error code
		}
		// TO DO: Check for -1 values
		while (read(pipeFd[PIPE_READ], buffer, sizeof(buffer)) > 0)
		{
			content += buffer;
			memset(buffer, '\0', sizeof(buffer));
		}
		if (close(pipeFd[PIPE_READ]) == -1)
		{
			// TO DO: Throw an appropiate exeception that caller class must transform into http error code
		}
		// TO DO: Check the status of waitpid
	}

	void Cgi::childProcess(
		int pipeFd[2],
		const std::string &path,
		const std::string &localPath,
		const std::map<std::string, std::string> &headers,
		const ConfigServer *config,
		const struct firstHeader &firstHeader) const
	{
		char *args[] = {
			(char *)this->_interpreter.c_str(),
			(char *)localPath.data(),
			NULL,
		};
		// TO DO: Add the SCRIPT_NAME env
		std::string contentLength = "CONTENT_LENGTH=";
		std::string contentType = "CONTENT_TYPE=";
		std::string gatewayInterface = "GATEWAY_INTERFACE=CGI/1.1";
		std::string httpAccept = "HTTP_ACCEPT=";
		std::string httpAcceptCharset = "HTTP_ACCEPT_CHARSET=";
		std::string httpAcceptEncoding = "HTTP_ACCEPT_ENCODING=";
		std::string httpAcceptLanguage = "HTTP_ACCEPT_LANGUAGE=";
		std::string httpHost = "HTTP_HOST=";
		std::string userAgent = "HTTP_USER_AGENT=";
		std::string pathInfo = "PATH_INFO=" + this->_pathInfo;
		std::string pathTranslated = "PATH_TRANSLATED=" + localPath;
		std::string queryString = "QUERY_STRING=" + firstHeader.query;
		std::string reqMethod = "REQUEST_METHOD=" + firstHeader.method.first;
		std::string scriptName = "SCRIPT_NAME=" + (this->_pathInfo.size() <= 0 ? path : path.substr(0, path.rfind(this->_pathInfo)));
		std::string serverName = "SERVER_NAME=" + config->getServerName();
		std::string port = "SERVER_PORT=" + config->getPort();
		std::string serverProtocol = "SERVER_PROTOCOL=HTTP";
		std::string httpCookie = "HTTP_COOKIE=";

		this->addHeaderValue(contentLength, "-1", "Content-Length", headers);
		this->addHeaderValue(contentType, "null", "Content-Type", headers);
		this->addHeaderValue(httpAccept, "", "Accept", headers);
		this->addHeaderValue(httpAcceptCharset, "", "Accept-Charset", headers);
		this->addHeaderValue(httpAcceptEncoding, "", "Accept-Encoding", headers);
		this->addHeaderValue(httpAcceptLanguage, "", "Accept-Language", headers);
		this->addHeaderValue(httpHost, "", "Host", headers);
		this->addHeaderValue(userAgent, "", "User-Agent", headers);
		this->addHeaderValue(httpCookie, "", "Cookie", headers);
		char *env[] = {
			(char *)contentLength.data(),
			(char *)contentType.data(),
			(char *)gatewayInterface.data(),
			(char *)httpAccept.data(),
			(char *)httpAcceptCharset.data(),
			(char *)httpAcceptEncoding.data(),
			(char *)httpAcceptLanguage.data(),
			(char *)httpHost.data(),
			(char *)userAgent.data(),
			(char *)pathInfo.data(),
			(char *)pathTranslated.data(),
			(char *)queryString.data(),
			(char *)reqMethod.data(),
			(char *)scriptName.data(),
			(char *)serverName.data(),
			(char *)port.data(),
			(char *)serverProtocol.data(),
			(char *)httpCookie.data(),
			NULL,
		};
		if (dup2(pipeFd[PIPE_READ], STDIN_FILENO) == -1)
		{
			close(pipeFd[PIPE_READ]);
			close(pipeFd[PIPE_WRITE]);
			// TO DO: Later do an exit of -1 for parent process to pick up
		}
		if (dup2(pipeFd[PIPE_WRITE], STDOUT_FILENO) == -1)
		{
			close(pipeFd[PIPE_READ]);
			close(pipeFd[PIPE_WRITE]);
			// TO DO: Later do an exit of -1 for parent process to pick up
		}
		if (close(pipeFd[PIPE_READ]) == -1)
		{
			close(pipeFd[PIPE_WRITE]);
			// TO DO: Later do an exit of -1 for parent process to pick up
		}
		if (close(pipeFd[PIPE_WRITE]) == -1)
		{
			// TO DO: Later do an exit of -1 for parent process to pick up
		}
		execve(this->_interpreter.c_str(), args, env);
		// TO DO: If we reach here, send an exit failure
	}

	void Cgi::addHeaderValue(
		std::string &env,
		std::string errorValue,
		const std::string &searchValue,
		const std::map<std::string, std::string> &headers) const
	{
		env += headers.find(searchValue) == headers.end() ? errorValue : headers.find(searchValue)->second;
	}

	const char *Cgi::NotFoundException::what(void) const throw()
	{
		return ("Cgi script not found");
	}

	const char *Cgi::CgiErrorException::what(void) const throw()
	{
		return ("Internal error with cgi script");
	}

	Cgi::~Cgi() {}
}