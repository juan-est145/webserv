/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:46:33 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/31 12:54:30 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HttpResponse.hpp"

namespace Webserv
{
    HttpResponse::HttpResponse(void)
    {
        this->_resCode = "200";
        this->_httpVersion = "HTTP/1.1";
        this->_headers["Server"] = "webserv/1.1";
    }

    HttpResponse::HttpResponse(const std::string &resCode, const std::string &httpVersion)
    {
        this->_resCode = resCode;
        this->_httpVersion = httpVersion;
        this->_headers["Server"] = "webserv/1.1";
    }
    
    HttpResponse::HttpResponse(HttpResponse &toCopy)
    {
        *this = toCopy;
    }

    HttpResponse &HttpResponse::operator=(const HttpResponse &toCopy)
    {
        if (this != &toCopy)
        {
            this->_resCode = toCopy._resCode;
            this->_httpVersion = toCopy._httpVersion;
            this->_headers = toCopy._headers;
        }
        return (*this);
    }

    std::string HttpResponse::returnResponse(const std::string &content) const
    {
        std::stringstream ss;
        
        ss << this->_httpVersion << " " << this->_resCode << "\r\n";
        for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
            ss << it->first << ": " << it->second << "\r\n";
        ss << "\r\n" << content;
        return (ss.str());
    }
 
    //*************GETTERS**************//
    
    const std::string &HttpResponse::getResCode(void) const
    {
        return (this->_resCode);
    }

    const std::string &HttpResponse::getHttpVersion(void) const
    {
        return (this->_httpVersion);
    }

    std::string HttpResponse::getLocation(void) const
    {
        std::map<std::string, std::string>::const_iterator it = this->_headers.find("Location");
        return it == this->_headers.end() ? "" : it->second;
    }

    std::string HttpResponse::getAllow(void) const
    {
        std::map<std::string, std::string>::const_iterator it = this->_headers.find("Allow");
        return it == this->_headers.end() ? "" : it->second;
    }

    const std::map<std::string, std::string> &HttpResponse::getHeaders(void) const
    {
        return (this->_headers);
    }
    

    /*std::string getWwwAuthenticate() const
    {
        return this->wwwAuthenticate;
    }*/
    
    /*int getRetryAfter() const
    {
        return this->retryAfter;
    }*/
    //*************SETTERS**************//
    void HttpResponse::setResCode(const std::string &resCode)
    {
        this->_resCode = resCode;
    }

    void HttpResponse::setHttpVersion(const std::string &httpVersion)
    {
        this->_httpVersion = httpVersion;
    }

    void HttpResponse::setHeaders(const std::map<std::string, std::string> &resHeaders)
    {
        std::map<std::string, std::string>::const_iterator it;
        for (it = resHeaders.begin(); it != resHeaders.end(); it++)
            this->_headers[it->first] = it->second;
    }

    void HttpResponse::setDate(const std::string &date)
    {
        this->_headers["Date"] = date;
    }

    HttpResponse::~HttpResponse() {}
}