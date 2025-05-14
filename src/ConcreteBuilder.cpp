/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:30:19 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/14 12:47:33 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ConcreteBuilder.hpp"

namespace Webserv
{
    ConcreteBuilder::ConcreteBuilder(void)
    {
        this->_response = new HttpResponse();
    }

    ConcreteBuilder::ConcreteBuilder(HttpResponse *response)
    {
        this->_response = response;
    }

    ConcreteBuilder::ConcreteBuilder(ConcreteBuilder &toCopy)
    {
        *this = toCopy;
    }

    ConcreteBuilder &ConcreteBuilder::operator=(const ConcreteBuilder &other)
    {
        if (this != &other)
            this->_response = other._response;
        return *this;
    }

    //*****GETTERS*****//
    HttpResponse *ConcreteBuilder::getResponse() const
    {
        return (this->_response);
    }

    //*****SETTERS*****//
    void ConcreteBuilder::setResCode(const std::string &resCode)
    {
        this->_response->setResCode(resCode);
    }

    void ConcreteBuilder::setMime(const std::string &mime)
    {
        this->_response->setMime(mime);
    }

    void ConcreteBuilder::setContentLength(int length)
    {
        this->_response->setContentLength(length);
    }

    void ConcreteBuilder::setLocation(const std::string &location)
    {
        this->_response->setLocation(location);
    }

    void ConcreteBuilder::setAllow(const std::string &location)
    {
        this->_response->setAllow(location);
    }

    void ConcreteBuilder::setDate(const std::string &date)
    {
        this->_response->setDate(date);
    }

    ConcreteBuilder::~ConcreteBuilder() {}
}
