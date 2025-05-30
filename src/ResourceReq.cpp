/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceReq.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:29:40 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/28 23:24:03 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ResourceReq.hpp"

namespace Webserv
{
	ResourceReq::ResourceReq(void) : AServerAction() {}

	ResourceReq::ResourceReq(const std::string path, const int &reqFd) : AServerAction(path, reqFd) {}

	ResourceReq::ResourceReq(const ResourceReq &copy) : AServerAction(copy)
	{
		*this = copy;
	}

	ResourceReq &ResourceReq::operator=(const ResourceReq &assign)
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

	void ResourceReq::processRequest(
		const ConfigServer *config, 
		const struct RequestData &reqData, 
		const std::map<std::string, Webserv::CookieData> &sessions)
	{
		try
		{
			this->handleCookies(reqData._reqHeader, reqData._firstHeader.path, reqData._firstHeader.method.first, sessions);
			this->obtainResource(config, reqData);
		}
		catch (const Webserv::AServerAction::HttpException &e)
		{
			this->_resCode = e.getResCode();
			this->processHttpError(config);
			this->setContentType("text/html");
		}
	}

	void ResourceReq::obtainResource(const ConfigServer *config, const struct RequestData &reqData)
	{
		struct stat fileStat;
		const Location locationFile = this->obtainLocationConf(config);
		std::string localPath = AuxFunc::mapPathToResource(locationFile, this->_path);

		this->isMethodAllowed(locationFile, reqData._firstHeader.method.first);
		if (reqData._reqBody.size() != 0)
			throw Webserv::AServerAction::HttpException(400);
		if (reqData._firstHeader.httpVers != "HTTP/1.1")
			throw Webserv::AServerAction::HttpException(505);
		if (locationFile.getReturn().size() > 0)
		{
			this->redirect(locationFile.getReturn(), config);
			return;
		}
		if (locationFile.getCgiPath().size() > 0)
		{
			if (this->isCgi(locationFile, config, reqData))
				return;
		}
		this->checkPermissions(localPath, fileStat);
		if (fileStat.st_mode & S_IFDIR)
		{
			if (locationFile.getAutoindex() == true)
			{
				this->directoryListing(localPath);
				return;
			}
			localPath += localPath[localPath.size() - 1] == '/' ? locationFile.getIndexLocation() : "/" + locationFile.getIndexLocation();
			this->checkPermissions(localPath, fileStat);
		}
		this->_size = fileStat.st_size;
		this->readResource(localPath);
		this->setContentType(this->chooseMime(localPath));
		this->setContentLength(this->_size);
	}

	std::string ResourceReq::chooseMime(const std::string &path) const
	{
		// NOTE: If it became necessary to add more mimes, the inclusion must respect the order value of the ascii table.
		// Numbers first, then uppercase and lastly, lowercase. This restrictions apply to the first key of the pair
		// Failing to comply with these instructions will cause the binaryMimeSearch to fail and create bugs.
		std::pair<std::string, std::string> mimes[] = {
			std::make_pair(".3gp", "video/3gpp"),
			std::make_pair(".3g2", "video/3gpp2"),
			std::make_pair(".7z", "application/x-7z-compressed"),
			std::make_pair(".aac", "audio/aac"),
			std::make_pair(".abw", "application/x-abiword"),
			std::make_pair(".arc", "application/octet-stream"),
			std::make_pair(".avi", "video/x-msvideo"),
			std::make_pair(".azw", "application/vnd.amazon.ebook"),
			std::make_pair(".bin", "application/octet-stream"),
			std::make_pair(".bz", "application/x-bzip"),
			std::make_pair(".bz2", "application/x-bzip2"),
			std::make_pair(".csh", "application/x-csh"),
			std::make_pair(".css", "text/css"),
			std::make_pair(".csv", "text/csv"),
			std::make_pair(".doc", "application/msword"),
			std::make_pair(".epub", "application/epub+zip"),
			std::make_pair(".gif", "image/gif"),
			std::make_pair(".htm", "text/html"),
			std::make_pair(".html", "text/html"),
			std::make_pair(".ico", "image/x-icon"),
			std::make_pair(".ics", "text/calendar"),
			std::make_pair(".jar", "application/java-archive"),
			std::make_pair(".jpeg", "image/jpeg"),
			std::make_pair(".jpg", "image/jpeg"),
			std::make_pair(".js", "application/javascript"),
			std::make_pair(".json", "application/json"),
			std::make_pair(".mid", "audio/midi"),
			std::make_pair(".midi", "audio/midi"),
			std::make_pair(".mpeg", "video/mpeg"),
			std::make_pair(".mpkg", "application/vnd.apple.installer+xml"),
			std::make_pair(".odp", "application/vnd.oasis.opendocument.presentation"),
			std::make_pair(".ods", "application/vnd.oasis.opendocument.spreadsheet"),
			std::make_pair(".odt", "application/vnd.oasis.opendocument.text"),
			std::make_pair(".oga", "audio/ogg"),
			std::make_pair(".ogv", "video/ogg"),
			std::make_pair(".ogx", "application/ogg"),
			std::make_pair(".pdf", "application/pdf"),
			std::make_pair(".ppt", "application/vnd.ms-powerpoint"),
			std::make_pair(".rar", "application/x-rar-compressed"),
			std::make_pair(".rtf", "application/rtf"),
			std::make_pair(".sh", "application/x-sh"),
			std::make_pair(".svg", "application/svg+xml"),
			std::make_pair(".swf", "application/x-shockwave-flash"),
			std::make_pair(".tar", "application/x-tar"),
			std::make_pair(".tif", "image/tiff"),
			std::make_pair(".tiff", "image/tiff"),
			std::make_pair(".ttf", "font/ttf"),
			std::make_pair(".vsd", "application/vnd.visio"),
			std::make_pair(".wav", "audio/x-wav"),
			std::make_pair(".weba", "audio/webm"),
			std::make_pair(".webm", "video/webm"),
			std::make_pair(".webp", "image/webp"),
			std::make_pair(".woff", "font/woff"),
			std::make_pair(".woff2", "font/woff2"),
			std::make_pair(".xhtml", "application/xhtml+xml"),
			std::make_pair(".xls", "application/vnd.ms-excel"),
			std::make_pair(".xml", "application/xml"),
			std::make_pair(".xul", "application/vnd.mozilla.xul+xml"),
			std::make_pair(".zip", "application/zip"),
		};
		std::size_t lastSlash = path.find_last_of("/");
		if (lastSlash == std::string::npos)
			lastSlash = 0;
		std::string fileName = path.substr(lastSlash);
		std::size_t extensionIndex = fileName.find_last_of(".");
		if (extensionIndex == std::string::npos)
			return ("text/plain");
		std::string extension = fileName.substr(extensionIndex);
		return (this->binaryMimeSearch(mimes, 0, sizeof(mimes) / sizeof(mimes[0]) - 1, extension).second);
	}

	std::pair<std::string, std::string> ResourceReq::binaryMimeSearch(
		std::pair<std::string, std::string> *mimes,
		int low,
		int high,
		std::string toFind) const
	{
		while (low <= high)
		{
			int mid = low + (high - low) / 2;
			if (mimes[mid].first == toFind)
				return (mimes[mid]);
			else if (mimes[mid].first < toFind)
				low = mid + 1;
			else
				high = mid - 1;
		}
		return (std::make_pair("default", "text/plain"));
	}

	void ResourceReq::directoryListing(const std::string &localPath)
	{
		DIR *dir = opendir(localPath.c_str());
		struct dirent *readDir;
		std::string title = "Index of " + this->_path;
		std::string skip[2] = {
			".",
			"..",
		};

		if (dir == NULL)
		{
			closedir(dir);
			Logger::errorLog(errno, strerror, false);
			throw Webserv::AServerAction::HttpException(500);
		}
		if (localPath[localPath.size() - 1] != '/')
		{
			closedir(dir);
			throw Webserv::AServerAction::HttpException(404);
		}
		this->_content += "<html>\n<head><title>" + title + "</title></head>\n";
		this->_content += "<body>\n<h1>" + title + "</h1><hr><pre><a href=\"../\">../</a>\n";
		while ((readDir = readdir(dir)) != NULL)
		{
			if (readDir->d_name == skip[0] || readDir->d_name == skip[1])
				continue;
			if (readDir->d_type == DT_DIR)
				this->addDirectoryInfo(readDir, localPath, dir);
			else
				this->addFileInfo(readDir, localPath, dir);
		}
		this->_content += "</pre><hr></body>\n</html>";
		if (closedir(dir) == -1)
			throw Webserv::AServerAction::HttpException(500);
		this->_size = this->_content.size();
		this->setContentType("text/html");
	}

	void ResourceReq::addDirectoryInfo(struct dirent *readDir, const std::string &localPath, DIR *dir)
	{
		std::stringstream link;
		struct stat fileStat;
		std::string directoryPath = localPath + readDir->d_name + "/";

		if (stat(directoryPath.c_str(), &fileStat) == -1)
		{
			closedir(dir);
			throw Webserv::AServerAction::HttpException(500);
		}
		link << "<a href=\"" << readDir->d_name << "/\">" << readDir->d_name << "/</a>";
		for (size_t i = 0; i < 41; i++)
			link << ' ';
		link << AuxFunc::getHumanTime(fileStat.st_mtime);
		for (size_t i = 0; i < 41; i++)
			link << ' ';
		link << "-\n";
		this->_content += link.str();
	}

	void ResourceReq::addFileInfo(struct dirent *readDir, const std::string &localPath, DIR *dir)
	{
		std::stringstream link;
		struct stat fileStat;
		std::string directoryPath = localPath + readDir->d_name;

		if (stat(directoryPath.c_str(), &fileStat) == -1)
		{
			closedir(dir);
			throw Webserv::AServerAction::HttpException(500);
		}
		link << "<a href=\"" << readDir->d_name << "\">" << readDir->d_name << "</a>";
		for (size_t i = 0; i < 41; i++)
			link << " ";
		link << AuxFunc::getHumanTime(fileStat.st_mtime);
		for (size_t i = 0; i < 41; i++)
			link << ' ';
		link << readDir->d_reclen << "\n";
		this->_content += link.str();
	}

	void ResourceReq::redirect(const std::string &uri, const ConfigServer *config)
	{
		this->_resCode = 301;
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

	void ResourceReq::checkPermissions(const std::string &localPath, struct stat &fileStat) const
	{
		if (access(localPath.c_str(), F_OK) == -1)
			throw Webserv::AServerAction::HttpException(404);
		else if (access(localPath.c_str(), R_OK) == -1 || stat(localPath.c_str(), &fileStat) == -1)
			throw Webserv::AServerAction::HttpException(500);
	}

	void ResourceReq::setContent(const std::string &_content)
	{
		this->_content = _content;
	}

	ResourceReq::~ResourceReq() {}
}
