/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:26:30 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/17 18:19:06 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <sys/stat.h>
#include "Request.hpp"

namespace Webserv
{
    class Request;

    class HttpResponse
    {
    private:
        std::string _resCode;
        std::string _httpVersion;
        std::map<std::string, std::string> _headers;

        int getFileSize(const char *filename);

    public:
        HttpResponse(void);
        HttpResponse(const std::string &resCode, const std::string &httpVersion);
        HttpResponse(HttpResponse &toCopy);
        HttpResponse &operator=(const HttpResponse &toCopy);

        std::string returnResponse(const Request *req) const;

        // Getters
        const std::string &getResCode(void) const;
        const std::string &getHttpVersion(void) const;
        std::string getLocation(void) const;
        std::string getAllow(void) const;
        const std::map<std::string, std::string> &getHeaders(void) const;

        // Setters
        void setResCode(const std::string &resCode);
        void setHttpVersion(const std::string &httpVersion);
        // void setMime(const std::string &mime);
        // void setContentLength(int contentLength);
        // void setLocation(const std::string &location);
        // void setAllow(const std::string &allow);
        void setHeaders(const std::map<std::string, std::string> &resHeaders);
        void setDate(const std::string &date);
        /*
        void setWwwAuthenticate(std::string wwwAuthenticate);


        void setRetryAfter(int retryAfter);
        */

        ~HttpResponse();
    };

}
#endif