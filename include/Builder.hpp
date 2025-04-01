/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Builder.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:15:04 by mfuente-          #+#    #+#             */
/*   Updated: 2025/04/01 12:32:10 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BUILDER_HPP
#define BUILDER_HPP

#include "./HttpResponse.hpp"
//ES UNA INTERFAZ POR ESO NO CREO UNA CLASE ORTODOXA CANONICA
namespace Webserv
{
    class Builder 
    {
        public:
            virtual ~Builder() {}
            virtual void SetStatus(const std::string& status) = 0;
            virtual void SetContent(const std::string& type) = 0;
            virtual void SetContentLength(int length) = 0;
            virtual HttpResponse* getResponse() const = 0;
    };
}



#endif