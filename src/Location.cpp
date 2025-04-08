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
        this->_clientMaxBodySize = MAX_CONTENT_LENGTH
    }
}