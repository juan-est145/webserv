/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:46:33 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/13 23:51:39 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HttpResponse.hpp"

namespace Webserv
{
    HttpResponse::HttpResponse(void)
    {
        this->status = "200";
        this->content = "text/html";
        this->contentLength = 0;
        /*
        this->wwwAuthenticate = NULL;
        this->location = NULL;
        this->allow = NULL;
        this->retryAfter = NULL;
        */
    }

    HttpResponse::HttpResponse(std::string status, std::string content, int contentLength)
    {
        this->status = status;
        this->content = content;
        this->contentLength = contentLength;
        /*
        this->wwwAuthenticate = wwwAuthenticate;
        this->location = location;
        this->allow = allow;
        this->retryAfter = retryAfter;
        */
    }
    HttpResponse::HttpResponse(HttpResponse &toCopy)
    {
        *this = toCopy;
    }

    HttpResponse &HttpResponse::operator=(const HttpResponse &other)
    {
        this->status = other.getStatus();
        this->content = other.getContent();
        this->contentLength = other.getContentLength();
        return *this;
    }

    std::string HttpResponse::Print(const Request *req) const
    {
        std::stringstream ss;
        ss << "HTTP/1.1 " << status << "\r\n"
           << "Content-Type: " << content << "\r\n"
           << "Content-Length: " << contentLength << "\r\n";
        if (req->getResCode() == 301 || req->getResCode() == 201)
            ss << "Location: " << req->getLocation() << "\r\n";
        /*      if (req->getResCode() == 401)
                    ss << "WWW-Authenticate: "<<req->getAutheticate() << "\r\n"; */
        /*      if (req->getResCode() == 405)
                    ss << "Allow: "<<req->getAllow() << "\r\n"; */
        /*      if (req->getResCode() == 503)
                    ss << "Retry-After: "<<req->getRetry() << "\r\n"; */
        ss << "\r\n"
           << req->getResourceContent();
        std::string response = ss.str();
        return (response);
    }
    // APARTADOS FALTANTES
    // WWW-Authenticate(401)
    // Location(301)(302)
    // OPCIONALES: ALLOW(405), RETRY-AFTER(503)
    //*************GETTERS**************//
    std::string HttpResponse::getStatus() const
    {
        return status;
    }

    std::string HttpResponse::getContent() const
    {
        return content;
    }

    int HttpResponse::getContentLength() const
    {
        return contentLength;
    }
    /*std::string getWwwAuthenticate() const
    {
        return this->wwwAuthenticate;
    }*/
    /*std::string getLocation() const
    {
        return this->location;
    }*/
    /*std::string getAllow() const
    {
        return this->allow;
    }*/
    /*int getRetryAfter() const
    {
        return this->retryAfter;
    }*/
    //*************SETTERS**************//
    void HttpResponse::setStatus(std::string status)
    {
        this->status = status;
    }

    void HttpResponse::setContent(std::string content)
    {
        this->content = content;
    }

    void HttpResponse::setContentLength(int contentLength)
    {
        this->contentLength = contentLength;
    }

    void HttpResponse::setLocation(const std::string &location)
    {
        this->location = location;
    }

    /*void setWwwAuthenticate(std::string wwwAuthenticate)
    {
        this->wwwAuthenticate = wwwAuthenticate;
    }*/

    /*void setAllow(std::string allow)
    {
        this->allow = allow;
    }*/
    /*void setRetryAfter(int retryAfter)
    {
        this->retryAfter = retryAfter;
    }*/
    HttpResponse::~HttpResponse() {}
}