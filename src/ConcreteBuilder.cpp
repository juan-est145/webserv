/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:30:19 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/13 18:36:45 by juestrel         ###   ########.fr       */
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
        if (this != &other)
            this->response = other.response;
        return *this;
    }

    //*****GETTERS*****//
    HttpResponse* ConcreteBuilder::getResponse() const
    {
        return (this->response);
    }
    
    //*****SETTERS*****//
    void ConcreteBuilder::SetStatus(const std::string& status)
    {
        this->response->setStatus(status);
    }

    void ConcreteBuilder::SetContent(const std::string& type)
    {
        this->response->setContent(type);
    }
    
    void ConcreteBuilder::SetContentLength(int length)
    {
        this->response->setContentLength(length);
    }

    ConcreteBuilder::~ConcreteBuilder() {}
}
