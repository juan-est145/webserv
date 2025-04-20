/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceReq.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:29:40 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/20 13:41:34 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ResourceReq.hpp"

namespace Webserv
{
	// TO DO: Later on we need a public function that takes as an argument the type of verb of the request
	ResourceReq::ResourceReq(void): AServerAction()
	{
		this->_resourceType = ResourceReq::REG_FILE;
	}

	ResourceReq::ResourceReq(const std::string path): AServerAction(path)  {}

	ResourceReq::ResourceReq(const ResourceReq &copy): AServerAction(copy)
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

	void ResourceReq::processRequest(ConfigServer *config)
	{

	}

	// int ResourceReq::obtainResource(const std::string &uri)
	// {
	// 	struct stat fileStat;
	// 	int responseCode;

	// 	// TO DO: Handle somehow 500 error codes later on
	// 	responseCode = this->mapUriToResource(uri);
	// 	if (access(this->_path.c_str(), R_OK) == -1)
	// 		responseCode = 500;
	// 	// TO DO: Implement exception here if stat fails
	// 	stat(this->_path.c_str(), &fileStat);
	// 	this->_size = fileStat.st_size;
	// 	this->readResource();
	// 	return (responseCode);
	// }

	// int ResourceReq::mapUriToResource(const std::string &uri)
	// {
	// 	// TODO: Once we have a working configuration file, we should use that information instead
	// 	int responseCode = 200;

	// 	// TODO: Later on I need to distinguish between type of files
	// 	this->_resourceType = ResourceReq::REG_FILE;
	// 	if (uri == "/")
	// 		this->_path = "./www/index.html";
	// 	else if (uri == "/upload")
	// 		this->_path = "./www/upload.html";
	// 	else
	// 	{
	// 		this->_path = "./www/error404.html";
	// 		responseCode = 404;
	// 	}
	// 	return (responseCode);
	// }

	void ResourceReq::readResource(void)
	{
		std::ifstream resource;
		char *buffer = NULL;

		resource.open(this->_path.c_str(), std::ios::in);
		buffer = new char[this->_size  + 1];
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
