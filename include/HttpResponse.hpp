/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:26:30 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/09 14:37:57 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <sys/stat.h>
#include "Request.hpp"

namespace Webserv
{
    class Request;

	class HttpResponse
	{
        private:
            std::string status;
            std::string content;
            int contentLength;
            //std::string wwwAuthenticate;
            //std::string location;
            //std::string allow;
            //int retryAfter;
            int getFileSize(const char* filename);
        public:
            HttpResponse(void);
            HttpResponse(std::string status, std::string content, int contentLength);
            HttpResponse(HttpResponse &toCopy);
            HttpResponse &operator=(const HttpResponse &other);
            ~HttpResponse();
            std::string Print(const Request *req) const;
            // Getters
            std::string getStatus() const;
            std::string getContent() const;
            int getContentLength() const;
            //std::string getWwwAuthenticate();
            //std::string getLocation();
            //std::string getAllow();
            //int getRetryAfter();

            // Setters
            void setStatus(std::string status);
            void setContent(std::string content);
            void setContentLength(int contentLength);
            /*
            void setWwwAuthenticate(std::string wwwAuthenticate);
            void setLocation(std::string location);
            void setAllow(std::string allow);
            void setRetryAfter(int retryAfter);
            */
    };
        
}
#endif