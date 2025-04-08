#include "../include/ConfigFile.hpp"

namespace Webserv
{
	ConfigFile::ConfigFile() : _path(NULL)
	{
		this->_path = "";
	}

	ConfigFile::ConfigFile(const std::string &path)
	{
		this->_path = path;
	}

	int ConfigFile::getPathType(const std::string &path)
	{
		struct stat buffer;
		int result;

		result = stat(path.c_str(), &buffer);
		if (result)
			return (PATH_ERROR);
		if (buffer.st_mode & S_IFREG)
			return (PATH_FILE);
		if (buffer.st_mode & S_IFDIR)
			return (PATH_FOLDER);
		return (PATH_OTHER);
	}

	int ConfigFile::fileOk(const std::string &path)
	{
		return (access(path.c_str(), R_OK | F_OK));
	}

	int ConfigFile::fileReadable(const std::string &path,
								 const std::string &index)
	{
		if (getPathType(index) == PATH_FILE &&
			!fileOk(index))
			return (0);
		if (getPathType(path + index) == PATH_FILE &&
			!fileOk(path + index))
			return (0);
		return (-1);
	}

	std::string ConfigFile::readFile(const std::string &path)
	{
		if (path.empty() || path.length() == 0)
			return ("");

		std::ifstream config_file(path.c_str());
		if (!config_file || !config_file.is_open())
			return ("");

		std::stringstream stream_save;
		stream_save << config_file.rdbuf();
		return (stream_save.str());
	}

	std::string ConfigFile::getPath()
	{
		return (this->_path);
	}

	ConfigFile::~ConfigFile() {}
}
