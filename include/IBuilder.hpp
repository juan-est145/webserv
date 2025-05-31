/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IBuilder.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:15:04 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/31 12:52:35 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILDER_HPP
#define BUILDER_HPP

#include <string>

namespace Webserv
{
    class HttpResponse;

    class IBuilder
    {
    protected:
        HttpResponse *_response;
    public:
        virtual void setResCode(const std::string &resCode) = 0;
        virtual void setDate(const std::string &date) = 0;
        virtual void setHeaders(const std::map<std::string, std::string> &resHeaders) = 0;

        virtual const HttpResponse *getResponse() const = 0;

        virtual ~IBuilder() {}
    };
}

#endif