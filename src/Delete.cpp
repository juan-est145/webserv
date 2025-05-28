/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:44:06 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/28 21:52:49 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Delete.hpp"

namespace Webserv
{
	Delete::Delete() : AServerAction()
	{
		this->_resCode = 204;
	}

	Delete::Delete(const std::string path) : AServerAction(path)
	{
		this->_resCode = 204;
	}

	Delete::Delete(const Delete &copy) : AServerAction(copy)
	{
		*this = copy;
	}
	
	Delete &Delete::operator=(const Delete &assign)
	{
		if (this != &assign)
		{
			this->_content = assign._content;
			this->_resCode = assign._resCode;
			this->_resHeaders = assign._resHeaders;
			this->_size = assign._size;
		}
		return (*this);
	}
	
	void Delete::deleteFile(const std::string &localPath)
	{
		if (std::remove(localPath.c_str()) == -1)
			throw Webserv::AServerAction::HttpException(500);
	}

	void Delete::processRequest(
		const ConfigServer *config,
		struct RequestData &reqData,
		const std::map<std::string, Webserv::CookieData> &sessions)
	{
		try
		{
			this->handleCookies(reqData._reqHeader, reqData._firstHeader.path, reqData._firstHeader.method.first, sessions);
			this->mainAction(config, reqData);
		}
		catch (const Webserv::AServerAction::HttpException &e)
		{
			this->_resCode = e.getResCode();
			this->processHttpError(config);
			this->setContentType("text/html");
		}
	}

	void Delete::mainAction(const ConfigServer *config, struct RequestData &reqData)
	{
		struct stat fileStat;
		const Location locationFile = this->obtainLocationConf(config);
		std::string localPath = AuxFunc::mapPathToResource(locationFile, this->_path);

		this->isMethodAllowed(locationFile, reqData._firstHeader.method.first);
		if (reqData._firstHeader.httpVers != "HTTP/1.1")
			throw Webserv::AServerAction::HttpException(505);
		if (locationFile.getReturn().size() > 0)
		{
			this->redirect(locationFile.getReturn(), config);
			return;
		}
		if (access(localPath.c_str(), F_OK) == -1)
			throw Webserv::AServerAction::HttpException(404);
		else if (access(localPath.c_str(), W_OK) == -1 || stat(localPath.c_str(), &fileStat) == -1)
			throw Webserv::AServerAction::HttpException(500);
		if (fileStat.st_mode & S_IFDIR)
			throw Webserv::AServerAction::HttpException(405);
		this->_size = 0;
		this->setContentLength(this->_size);
		this->deleteFile(localPath);
	}

	void Delete::redirect(const std::string &uri, const ConfigServer *config)
	{
		this->_resCode = 308;
		this->setContentType("text/html");
		if (uri.substr(0, 7) == "http://" || uri.substr(0, 8) == "https://")
		{
			this->_resHeaders["Location"] = uri;
			return;
		}
		this->_resHeaders["Location"] = "http://" + config->getServerName();
		this->_resHeaders["Location"] += config->getPort() == 80 ? "" : ":" + AuxFunc::ft_itoa(config->getPort());
		this->_resHeaders["Location"] += uri;
	}

	Delete::~Delete() {}
}