/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:46:33 by mfuente-          #+#    #+#             */
/*   Updated: 2025/03/20 18:23:22 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HttpResponse.hpp"


namespace Webserv
{
    HttpResponse::HttpResponse(void)
    {
        this->status = 200;
        this->content = "htpp://localhost:3000/";
        this->contentLength = getFileSize("../html/index.html");
    }


    HttpResponse::HttpResponse(std::string status, std::string content, int contentLength)
    {
        this->status = status;
        this->content = content;
        this->contentLength = contentLength;
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
    HttpResponse::~HttpResponse(){}

    int HttpResponse::getFileSize(const char* filename) 
    {
        struct stat st;
        if (stat(filename, &st) == 0)
            return static_cast<int>(st.st_size);
        return -1;
    }


    std::string HttpResponse::Print() const
    {
        std::stringstream ss;
        ss << "HTTP/1.1 " << status << "\n"
        << "Content-Type: " << content << "\n"
        << "Content-Length: " << contentLength << "\n\n";
        std::string header = ss.str();
        return header;
    }
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
}