/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:30:19 by mfuente-          #+#    #+#             */
/*   Updated: 2025/04/08 18:20:21 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ConcreteBuilder.hpp"

namespace Webserv
{
    ConcreteBuilder::ConcreteBuilder(void)
    {
        this->response = new HttpResponse();
    }
    ConcreteBuilder::ConcreteBuilder(HttpResponse *response)
    {
        this->response = response;
    }
    ConcreteBuilder::ConcreteBuilder(ConcreteBuilder &toCopy)
    {
        *this = toCopy;
    }
    ConcreteBuilder &ConcreteBuilder::operator=(const ConcreteBuilder& other)
    {
        this->response = other.getResponse();
        return *this;
    }
    ConcreteBuilder::~ConcreteBuilder()
    {
        //delete this->response;
    }

    //*****GETTERS*****//
    HttpResponse* ConcreteBuilder::getResponse() const
    {
        HttpResponse *result = this->response;
        new HttpResponse();
        return result;
    }
    //*****SETTERS*****//
    void ConcreteBuilder::SetStatus(const std::string& status)
    {
        response->setStatus(status);
    }   
    void ConcreteBuilder::SetContent(const std::string& type)
    {
        response->setContent(type);
    }
    void ConcreteBuilder::SetContentLength(int length)
    {
        response->setContentLength(length);
    }
}
