/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:26:30 by mfuente-          #+#    #+#             */
/*   Updated: 2025/04/19 13:56:50 by juestrel         ###   ########.fr       */
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
            // Setters
            void setStatus(std::string status);
            void setContent(std::string content);
            void setContentLength(int contentLength);
    };
        
}

#endif