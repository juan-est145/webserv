/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:05:43 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/14 12:04:23 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONCRETEBUILDER_HPP
#define CONCRETEBUILDER_HPP

#include "HttpResponse.hpp"
#include "IBuilder.hpp"

namespace Webserv
{
    class HttpResponse;

    class ConcreteBuilder : public IBuilder
    {
    public:
        ConcreteBuilder(void);
        ConcreteBuilder(HttpResponse *response);
        ConcreteBuilder(ConcreteBuilder &toCopy);
        ConcreteBuilder &operator=(const ConcreteBuilder &other);

        HttpResponse *getResponse() const;

        void setStatus(const std::string &status);
        void setResCode(const std::string &resCode);
        void setMime(const std::string &mime);
        void setContentLength(int length);
        void setLocation(const std::string &location);
        void setAllow(const std::string &allow);

        ~ConcreteBuilder();
    };
}

#endif