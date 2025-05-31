/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:30:19 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/31 12:53:13 by juestrel         ###   ########.fr       */
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
    const HttpResponse *ConcreteBuilder::getResponse() const
    {
        return (this->_response);
    }

    //*****SETTERS*****//
    void ConcreteBuilder::setResCode(const std::string &resCode)
    {
        this->_response->setResCode(resCode);
    }

    void ConcreteBuilder::setDate(const std::string &date)
    {
        this->_response->setDate(date);
    }

    void ConcreteBuilder::setHeaders(const std::map<std::string, std::string> &resHeaders)
    {
        this->_response->setHeaders(resHeaders);
    }

    ConcreteBuilder::~ConcreteBuilder() {}
}
