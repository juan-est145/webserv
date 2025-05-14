/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IBuilder.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:15:04 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/14 13:59:07 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <string>

// ES UNA INTERFAZ POR ESO NO CREO UNA CLASE ORTODOXA CANONICA
namespace Webserv
{
    class HttpResponse;

    class IBuilder
    {
    protected:
        HttpResponse *_response;
    public:
        virtual void setMime(const std::string &mime) = 0;
        virtual void setResCode(const std::string &resCode) = 0;
        virtual void setContentLength(int length) = 0;
        virtual void setLocation(const std::string &location) = 0;
        virtual void setAllow(const std::string &allow) = 0;
        virtual void setDate(const std::string &date) = 0;

        virtual const HttpResponse *getResponse() const = 0;

        virtual ~IBuilder() {}
    };
}

#endif