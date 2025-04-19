#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>
#include <map>
#include "AuxFunc.hpp"

namespace Webserv
{
    class Location
    {
    public:
        std::map<std::string, std::string> _extPath;
        // enum E_Methods
        // {
        //     GET = 0,
        //     POST = 1,
        //     DELETE = 2,
        // };

        Location();
        Location(const Location &toCopy);
        Location &operator=(const Location &toCopy);
        void setPath(std::string parametr);
        void setRootLocation(std::string parametr);
        void setMethods(const std::vector<std::string> &methods);
        void setAutoindex(std::string parametr);
        void setIndexLocation(std::string parametr);
        void setReturn(std::string parametr);
        void setAlias(std::string parametr);
        void setCgiPath(std::vector<std::string> path);
        void setCgiExtension(std::vector<std::string> extension);
        void setMaxBodySize(std::string parametr);
        void setMaxBodySize(unsigned long parametr);

        const std::string &getPath() const;
        const std::string &getRootLocation() const;
        const std::map<std::string, bool> &getMethods() const;
        const bool &getAutoindex() const;
        const std::string &getIndexLocation() const;
        const std::string &getReturn() const;
        const std::string &getAlias() const;
        const std::vector<std::string> &getCgiPath() const;
        const std::vector<std::string> &getCgiExtension() const;
        const std::map<std::string, std::string> &getExtensionPath() const;
        const unsigned long &getMaxBodySize() const;
        // std::string getPrintMethods() const; // for checking only
        ~Location();

    private:
        std::string _path;
        std::string _root;
        bool _autoindex;
        std::string _index;
        std::string _return;
        std::string _alias;
        std::vector<std::string> _cgiPath;
        std::vector<std::string> _cgiExt;
        unsigned long _clientMaxBodySize;
        std::map<std::string, bool> _allowedMethods;
    };
}

#endif
