/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceReq.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:29:40 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/20 18:45:01 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ResourceReq.hpp"

namespace Webserv
{
	// TO DO: Later on we need a public function that takes as an argument the type of verb of the request
	ResourceReq::ResourceReq(void) : AServerAction()
	{
		this->_resourceType = ResourceReq::REG_FILE;
	}

	ResourceReq::ResourceReq(const std::string path) : AServerAction(path)
	{
		this->_resourceType = ResourceReq::REG_FILE;
	}

	ResourceReq::ResourceReq(const ResourceReq &copy) : AServerAction(copy)
	{
		*this = copy;
	}

	ResourceReq &ResourceReq::operator=(const ResourceReq &assign)
	{
		if (this != &assign)
		{
			this->_content = assign._content;
			this->_size = assign._size;
			this->_resourceType = ResourceReq::REG_FILE;
		}
		return (*this);
	}

	void ResourceReq::processRequest(const ConfigServer *config)
	{
		this->obtainResource(config);
	}

	void ResourceReq::obtainResource(const ConfigServer *config)
	{
		struct stat fileStat;
		const Location locationFile = this->obtainLocationConf(config);
		std::string localPath = this->mapPathToResource(locationFile);

		if (access(localPath.c_str(), F_OK) == -1)
		{
			this->_resCode = 404;
			localPath = config->getRoot() + config->getErrorPages().find(this->_resCode)->second;
		}
		// TO DO: Handle somehow 500 error codes later on
		else if (access(localPath.c_str(), R_OK) == -1)
			this->_resCode = 500;
		// TO DO: Implement exception here if stat fails
		// TO DO: Implement 404 if stat does not find anything
		stat(localPath.c_str(), &fileStat);
		if (fileStat.st_mode & S_IFDIR)
		{
			if (locationFile.getAutoindex() == true)
				std::cout << "Nothing yet, will implement later" << std::endl;
			localPath += localPath[localPath.size() - 1] == '/' ? locationFile.getIndexLocation() : "/" + locationFile.getIndexLocation();
			stat(localPath.c_str(), &fileStat);
		}
		else
			std::cout << "We hit something else" << std::endl;
		this->_size = fileStat.st_size;
		this->readResource(localPath);
	}

	const Location &ResourceReq::obtainLocationConf(const ConfigServer *config) const
	{
		const std::vector<Webserv::Location> locations = config->getLocations();
		unsigned int locIndex = 0;
		unsigned int matchingChars = 0;
		unsigned int maxMatch = 0;

		for (unsigned int i = 0; i < locations.size(); i++)
		{
			matchingChars = 0;
			std::string path = locations[i].getPath();
			for (unsigned int letter = 0; letter < std::min(path.size(), this->_path.size()); letter++)
			{
				if (path[letter] != this->_path[letter])
					break;
				matchingChars++;
			}
			if (matchingChars > maxMatch)
			{
				maxMatch = matchingChars;
				locIndex = i;
			}
		}
		return (locations[locIndex]);
	}

	std::string ResourceReq::mapPathToResource(const Location &locationFile) const
	{
		std::string reqPath = this->_path;
		std::string path = locationFile.getPath().size() <= 1 ? "" : locationFile.getPath().substr(1);
		std::string resourcePath;
		unsigned int breakIndex = 0;

		for (; breakIndex < std::min(this->_path.size(), locationFile.getPath().size()); breakIndex++)
		{
			if (this->_path[breakIndex] != locationFile.getPath()[breakIndex])
				break;
		}
		reqPath.erase(0, breakIndex);
		resourcePath = AuxFunc::urldecode((locationFile.getRootLocation() + path + reqPath).c_str());
		return (resourcePath);
	}

	void ResourceReq::readResource(const std::string &path)
	{
		std::ifstream resource;
		char *buffer = NULL;

		resource.open(path.c_str(), std::ios::in);
		buffer = new char[this->_size + 1];
		// TO DO: Check that resource is open and throw exception if it is not
		resource.read(buffer, this->_size);
		buffer[this->_size] = '\0';
		this->_content = buffer;
		delete[] buffer;
	}

	void ResourceReq::setContent(const std::string &_content)
	{
		this->_content = _content;
	}

	ResourceReq::~ResourceReq() {}
}
