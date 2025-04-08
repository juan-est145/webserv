#include "../include/Location.hpp"

namespace Webserv
{
    Location::Location()
    {
        this->_path = "";
        this->_root = "";
        this->_autoindex = false;
        this->_index = "";
        this->_return = "";
        this->_alias = "";
        this->_clientMaxBodySize = MAX_CONTENT_LENGTH;
        this->_allowedMethods["GET"] = false;
        this->_allowedMethods["POST"] = false;
        this->_allowedMethods["DELETE"] = false;
    }

    Location::Location(const Location &toCopy)
    {
        this->_path = toCopy._path;
        this->_root = toCopy._root;
        this->_autoindex = toCopy._autoindex;
        this->_index = toCopy._index;
        this->_return = toCopy._return;
        this->_alias = toCopy._alias;
        this->_clientMaxBodySize = toCopy._clientMaxBodySize;
        this->_allowedMethods = toCopy._allowedMethods;
        this->_cgiExt = toCopy._cgiExt;
        this->_cgiPath = toCopy._cgiPath;
    }

    Location &Location::operator=(const Location &toCopy)
    {
        if (this != &toCopy)
        {
            this->_path = toCopy._path;
            this->_root = toCopy._root;
            this->_autoindex = toCopy._autoindex;
            this->_index = toCopy._index;
            this->_return = toCopy._return;
            this->_alias = toCopy._alias;
            this->_clientMaxBodySize = toCopy._clientMaxBodySize;
            this->_allowedMethods = toCopy._allowedMethods;
            this->_cgiExt = toCopy._cgiExt;
            this->_cgiPath = toCopy._cgiPath;
        }
        return (*this);
    }

    void Location::setPath(std::string parameter)
    {
        this->_path = parameter;
    }

    void Location::setRootLocation(std::string parameter)
    {
        if (ConfigFile::getPathType(parameter) != ConfigFile::PATH_FOLDER)
            throw ConfigServer::ErrorException("root of location");
        this->_root = parameter;
    }

    void Location::setMethods(const std::vector<std::string> &methods)
    {
        for (size_t i = 0; i < methods.size(); i++)
        {
            if (methods[i] != "GET" || methods[i] != "POST" || methods[i] != "DELETE")
                throw ConfigServer::ErrorException("Allow method not supported " + methods[i]);
            this->_allowedMethods[methods[i]] = true;
        }
    }

    void Location::setAutoindex(std::string parameter)
    {
        if (parameter == "on" || parameter == "off")
            this->_autoindex = parameter == "on" ? true : false;
        else
            throw ConfigServer::ErrorException("Wrong autoindex");
    }

    void Location::setReturn(std::string parameter)
    {
        this->_return = parameter;
    }

    void Location::setAlias(std::string parameter)
    {
        this->_alias = parameter;
    }

    void Location::setAlias(std::string parameter)
    {
        this->_alias = parameter;
    }

    void Location::setCgiPath(std::vector<std::string> path)
    {
        this->_cgiPath = path;
    }

    void Location::setCgiPath(std::vector<std::string> path)
    {
        this->_cgiPath = path;
    }

    void Location::setCgiExtension(std::vector<std::string> extension)
    {
        this->_cgiExt = extension;
    }

    Location::~Location() {}

}