/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:05:43 by mfuente-          #+#    #+#             */
/*   Updated: 2025/04/19 14:08:00 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONCRETEBUILDER_HPP
#define CONCRETEBUILDER_HPP

#include "HttpResponse.hpp"
#include "Builder.hpp"

namespace Webserv
{
    class HttpResponse;

    class ConcreteBuilder : public Builder
    {
    public:
        ConcreteBuilder(void);
        ConcreteBuilder(HttpResponse *response);
        ConcreteBuilder(ConcreteBuilder &toCopy);
        ConcreteBuilder &operator=(const ConcreteBuilder &other);
        ~ConcreteBuilder();
        void SetStatus(const std::string &status);
        void SetContent(const std::string &type);
        void SetContentLength(int length);
        HttpResponse *getResponse() const;
    };
}

#endif