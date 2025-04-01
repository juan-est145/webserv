/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConcreteBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:05:43 by mfuente-          #+#    #+#             */
/*   Updated: 2025/03/27 18:40:33 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONCRETEBUILDER_HPP
#define CONCRETEBUILDER_HPP

#include "Builder.hpp"

namespace Webserv
{
    class ConcreteBuilder : public Builder {
        private:
            HttpResponse *response;
        public:
            ConcreteBuilder(void);
            ConcreteBuilder(HttpResponse *response);
            ConcreteBuilder(ConcreteBuilder &toCopy);
            ConcreteBuilder &operator=(const ConcreteBuilder& other);
            ~ConcreteBuilder();
            void SetStatus(const std::string& status) ;
            void SetContent(const std::string& type) ;
            void SetContentLength(int length) ;
            HttpResponse* getResponse() const ;
    };
}



#endif