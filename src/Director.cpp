/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/04/19 14:22:45 by juestrel         ###   ########.fr       */
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
    void Director::BuildOkResponse(long size)
    {
        builder->SetStatus("200");
        builder->SetContent("text/html");
        builder->SetContentLength(size);
    }
    //CODE 201
    void Director::BuildOkUploadResponse(long size) //toco content_length
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