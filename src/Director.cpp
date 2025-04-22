/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/04/22 13:43:48 by juestrel         ###   ########.fr       */
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
    void Director::BuildErrorResponse(long size, unsigned int resCode)
    {
        if (resCode < 400)
            throw std::exception();
        this->builder->SetStatus(AuxFunc::ft_itoa(resCode));
        this->builder->SetContent("text/html");
        this->builder->SetContentLength(size);
    }
}