/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:13:04 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/29 23:21:51 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cgi.hpp"

namespace Webserv
{
	Cgi::Cgi() : _locationConf(NULL), _reqData(NULL)
	{
		this->_interpreter = "";
		this->_pathInfo = "";
	}

	Cgi::Cgi(const Location &location, const struct RequestData &reqData) : _locationConf(&location), _reqData(&reqData)
	{
		this->_interpreter = "";
		this->_pathInfo = "";
	}

	Cgi::Cgi(const Cgi &toCopy) : _locationConf(toCopy._locationConf), _reqData(toCopy._reqData)
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

	bool Cgi::canProcessAsCgi(const ConfigServer *config, const int reqFd)
	{
		std::string cgiPath(this->_reqData->_firstHeader.path);
		std::vector<std::string> segmentedPath = this->obtainSegmentedPath(cgiPath);
		std::pair<cgiExtenIndex, urlSegmentIndex> indexes = this->selectCgiExtensions(segmentedPath);
		if (!this->isValidCgiRoute(cgiPath, segmentedPath, indexes))
			return (false);
		this->extractPathInfoAndInter(indexes, cgiPath, segmentedPath);
		this->execCgi(cgiPath, this->findCgiFile(cgiPath, segmentedPath, indexes), config, reqFd);
		return (true);
	}

	std::vector<std::string> Cgi::obtainSegmentedPath(const std::string &path) const
	{
		std::vector<std::string> segmentedPath;
		size_t pos = 0;
		const std::string delimiter = "/";
		std::string copy(path);

		while ((pos = copy.find(delimiter)) != std::string::npos)
		{
			segmentedPath.push_back(copy.substr(0, pos));
			copy.erase(0, pos + delimiter.size());
		}
		if (copy.size() > 0)
			segmentedPath.push_back(copy);
		return (segmentedPath);
	}

	bool Cgi::isValidCgiRoute(std::string &path, std::vector<std::string> &segmentedPath, std::pair<cgiExtenIndex, urlSegmentIndex> &indexes) const
	{
		struct stat fileStat;
		std::string localPath = AuxFunc::mapPathToResource(*this->_locationConf, path);

		if (indexes.first != -1 && indexes.second != -1)
			return (true);
		if (access(localPath.c_str(), F_OK) == -1)
			throw Webserv::Cgi::NotFoundException();
		if (stat(localPath.c_str(), &fileStat) == -1)
			throw Webserv::Cgi::CgiErrorException();
		if (fileStat.st_mode & S_IFREG || this->_locationConf->getAutoindex())
			return (false);
		path += path[path.size() - 1] == '/' ? this->_locationConf->getIndexLocation() : "/" + this->_locationConf->getIndexLocation();
		segmentedPath = this->obtainSegmentedPath(path);
		indexes = this->selectCgiExtensions(segmentedPath);
		if (indexes.first == -1 || indexes.second == -1)
			return (false);
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
		const std::string &cgiPath, 
		const std::string &localPath,
		const ConfigServer *config,
		const int reqFd) const
	{
		int pipeFd[2];

		if (pipe(pipeFd) == -1)
			throw Webserv::Cgi::CgiErrorException();
		pid_t pid = fork();
		if (pid == -1)
		{
			close(pipeFd[PIPE_READ]);
			close(pipeFd[PIPE_WRITE]);
			throw Webserv::Cgi::CgiErrorException();
		}
		else if (pid == 0)
			this->childProcess(pipeFd, cgiPath, localPath, config);
		this->parentProcess(pipeFd, pid, reqFd);
	}

	void Cgi::childProcess(
		int pipeFd[2],
		const std::string &cgiPath,
		const std::string &localPath,
		const ConfigServer *config) const
	{
		char *args[] = {
			(char *)this->_interpreter.c_str(),
			(char *)localPath.data(),
			NULL,
		};
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
		std::string queryString = "QUERY_STRING=" + this->_reqData->_firstHeader.query;
		std::string reqMethod = "REQUEST_METHOD=" + this->_reqData->_firstHeader.method.first;
		std::string scriptName = "SCRIPT_NAME=" + (this->_pathInfo.size() <= 0 ? cgiPath : cgiPath.substr(0, cgiPath.rfind(this->_pathInfo)));
		std::string serverName = "SERVER_NAME=" + config->getServerName();
		std::string port = "SERVER_PORT=" + AuxFunc::ft_itoa(config->getPort());
		std::string serverProtocol = "SERVER_PROTOCOL=HTTP";
		std::string httpCookie = "HTTP_COOKIE=";

		this->addHeaderValue(contentLength, AuxFunc::ft_itoa((unsigned int)this->_reqData->_reqBody.size()), "Content-Length", this->_reqData->_reqHeader);
		this->addHeaderValue(contentType, "null", "Content-Type", this->_reqData->_reqHeader);
		this->addHeaderValue(httpAccept, "", "Accept", this->_reqData->_reqHeader);
		this->addHeaderValue(httpAcceptCharset, "", "Accept-Charset", this->_reqData->_reqHeader);
		this->addHeaderValue(httpAcceptEncoding, "", "Accept-Encoding", this->_reqData->_reqHeader);
		this->addHeaderValue(httpAcceptLanguage, "", "Accept-Language", this->_reqData->_reqHeader);
		this->addHeaderValue(httpHost, "", "Host", this->_reqData->_reqHeader);
		this->addHeaderValue(userAgent, "", "User-Agent", this->_reqData->_reqHeader);
		this->addHeaderValue(httpCookie, "", "Cookie", this->_reqData->_reqHeader);
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
			exit(EXIT_FAILURE);
		}
		if (dup2(pipeFd[PIPE_WRITE], STDOUT_FILENO) == -1)
		{
			close(pipeFd[PIPE_READ]);
			close(pipeFd[PIPE_WRITE]);
			exit(EXIT_FAILURE);
		}
		if (close(pipeFd[PIPE_READ]) == -1)
		{
			close(pipeFd[PIPE_WRITE]);
			exit(EXIT_FAILURE);
		}
		if (close(pipeFd[PIPE_WRITE]) == -1)
			exit(EXIT_FAILURE);
		execve(this->_interpreter.c_str(), args, env);
		exit(EXIT_FAILURE);
	}

	void Cgi::addHeaderValue(
		std::string &env,
		std::string errorValue,
		const std::string &searchValue,
		const std::map<std::string, std::string> &headers) const
	{
		env += headers.find(searchValue) == headers.end() ? errorValue : headers.find(searchValue)->second;
	}

	void Cgi::parentProcess(int pipeFd[2], pid_t &pid, const int reqFd) const
	{
		//int status = 0;
		
		(void)pid;

		if (write(pipeFd[PIPE_WRITE], this->_reqData->_reqBody.c_str(), this->_reqData->_reqBody.size()) == -1)
		{
			close(pipeFd[PIPE_WRITE]);
			close(pipeFd[PIPE_READ]);
			throw Webserv::Cgi::CgiErrorException();
		}
		if (close(pipeFd[PIPE_WRITE]) == -1)
		{
			close(pipeFd[PIPE_READ]);
			throw Webserv::Cgi::CgiErrorException();
		}
		std::cout << "We have written to " << pipeFd[PIPE_WRITE] << " this text " << this->_reqData->_reqBody << std::endl;
		ARequest *cgiReq = new CgiReq(pipeFd, reqFd, pid);
		IServer *server = ICluster::cluster->findServer(reqFd);
		if (!AuxFunc::handle_ctl(ICluster::cluster->getEpollFd(), EPOLL_CTL_DEL, EPOLLIN, reqFd, ICluster::cluster->getEvent()))
			throw Webserv::IServer::ServerException();
		if (!AuxFunc::handle_ctl(ICluster::cluster->getEpollFd(), EPOLL_CTL_ADD, EPOLLIN, pipeFd[PIPE_READ], ICluster::cluster->getEvent()))
			throw Webserv::IServer::ServerException();
		ICluster::cluster->addPipeSocket(pipeFd[PIPE_READ], server);
		server->addClientPool(pipeFd[PIPE_READ], cgiReq);
		// if (waitpid(pid, &status, 0) == -1)
		// {
		// 	close(pipeFd[PIPE_WRITE]);
		// 	close(pipeFd[PIPE_READ]);
		// 	throw Webserv::Cgi::CgiErrorException();
		// }
		// while ((bytesRead = read(pipeFd[PIPE_READ], buffer, sizeof(buffer))) > 0)
		// {
		// 	content += buffer;
		// 	memset(buffer, '\0', sizeof(buffer));
		// }
		// if (bytesRead == -1)
		// {
		// 	close(pipeFd[PIPE_READ]);
		// 	throw Webserv::Cgi::CgiErrorException();
		// }
		// if (close(pipeFd[PIPE_READ]) == -1)
		// 	throw Webserv::Cgi::CgiErrorException();
		// if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		// 	throw Webserv::Cgi::CgiErrorException();
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