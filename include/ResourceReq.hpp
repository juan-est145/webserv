/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceReq.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:26:04 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/28 23:07:41 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESOURCEREQ_HPP
#define RESOURCEREQ_HPP

#include <iostream>
#include <string>
#include <sys/stat.h>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <algorithm>
#include <utility>
#include <sys/types.h>
#include <dirent.h>
#include <sstream>
#include "AServerAction.hpp"
#include "Request.hpp"
#include "Cookie.hpp"

namespace Webserv
{
	class Request;

	class ResourceReq : public AServerAction
	{
	private:
		void obtainResource(const ConfigServer *config, struct RequestData &reqData);
		std::string chooseMime(const std::string &path) const;
		std::pair<std::string, std::string> binaryMimeSearch(
			std::pair<std::string, std::string> *mimes,
			int low, int high,
			std::string toFind) const;
		void directoryListing(const std::string &localPath);
		void addDirectoryInfo(struct dirent *readDir, const std::string &localPath, DIR *dir);
		void addFileInfo(struct dirent *readDir, const std::string &localPath, DIR *dir);
		void redirect(const std::string &uri, const ConfigServer *config);
		void checkPermissions(const std::string &localPath, struct stat &fileStat) const;

	public:
		ResourceReq(void);
		ResourceReq(const std::string path, const int &reqFd);
		ResourceReq(const ResourceReq &copy);
		ResourceReq &operator=(const ResourceReq &assign);

		void processRequest(
			const ConfigServer *config,
			struct RequestData &reqData,
			const std::map<std::string, Webserv::CookieData> &sessions);

		void setContent(const std::string &_content);
		~ResourceReq();
	};
}

#endif