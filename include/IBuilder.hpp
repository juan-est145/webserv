/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:15:04 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/13 23:55:50 by juestrel         ###   ########.fr       */
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
        HttpResponse *response;
    public:
        virtual void SetStatus(const std::string &status) = 0;
        virtual void SetContent(const std::string &type) = 0;
        virtual void SetContentLength(int length) = 0;
        virtual void setLocation(const std::string &location) = 0;

        virtual HttpResponse *getResponse() const = 0;

        virtual ~IBuilder() {}
    };
}

#endif