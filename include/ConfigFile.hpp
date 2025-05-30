/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:49:01 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/21 21:49:02 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <exception>
#include "Location.hpp"

namespace Webserv
{
	class ConfigFile
	{
	private:
		std::string _path;

	public:
		enum E_PathType
		{
			PATH_ERROR = -1,
			PATH_FILE = 0,
			PATH_FOLDER = 1,
			PATH_OTHER = 2
		};
		ConfigFile();
		ConfigFile(const std::string &path);
		/**
		 * @return -1 if error, 0 if file, 1 if folder,
		 * 2 if other
		 */
		static int getPathType(const std::string &path);

		/**
		 * Checks if path is a valid file
		 * @return -1 if not, 0 if ok
		 */
		static int fileOk(const std::string &path);

		/**
		 * Checks if index or path + index are valid files
		 * @return -1 if not, 0 if ok
		 */
		static int fileReadable(const std::string &path, const std::string &index);

		/**
		 * @return string if path ok, NULL if path not ok
		 */
		std::string readFile(const std::string &path);

		static int checkFile(const std::string &path, int mode);

		/**
		 * @return _path
		 */
		std::string getPath();
		~ConfigFile();
	};
}

#endif
