/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/04/08 13:03:20 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Director.hpp"

namespace Webserv
{
    Director::Director(void)
    {
        this->builder = NULL;
    }
    Director::Director(Builder *builder)
    {
        this->builder = builder;
    }

    Director::Director(Director &toCopy)
    {
        *this = toCopy;
    }

    Director &Director::operator=(const Director& other)
    {
        this->builder = other.builder;
        return *this;
    }
    Director::~Director()
    {}
    void Director::SetBuilder(Builder *builder)
    {
        this->builder = builder;   
    }
    //CODE 200
    void Director::BuildOkResponse(const char* path)
    {
        builder->SetStatus("200");
        builder->SetContent("text/html");

        char result[std::strlen(path) + std::strlen("html")];
        std::strcpy(result, "html");
        if (std::strcmp(path, "/") != 0)
        {
            std::strcat(result, path);
            std::strcat(result, ".html");
        }
        else
            std::strcat(result, "/index.html");

        builder->SetContentLength(getFileSize(result));
    }
    //CODE 201
    void Director::BuildOkUploadResponse(int size) //toco content_length
    {
        this->builder->SetStatus("201");
        this->builder->SetContent("text/html");
        this->builder->SetContentLength(size);
    }
    //CODE 404        
    void Director::BuildNotFoundResponse()
    {
        this->builder->SetStatus("404");
        this->builder->SetContent("text/html");
        this->builder->SetContentLength(getFileSize("html/error404.html"));
    }

    int getFileSize(const char* filename) 
    {
        struct stat st;
        if (stat(filename, &st) == 0)
            return static_cast<int>(st.st_size);
        return -1;
    }
}