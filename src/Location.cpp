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
        this->_allowedMethods[E_Methods::GET] = false;
        this->_allowedMethods[E_Methods::POST] = false;
        this->_allowedMethods[E_Methods::DELETE] = false;
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

    Location::~Location() {}

}