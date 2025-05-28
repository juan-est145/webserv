/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:56:55 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/29 00:28:20 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

#include <sys/stat.h>
#include "AuxFunc.hpp"
#include "IBuilder.hpp"

namespace Webserv
{
    class Builder;

    class Director
    {
    private:
        IBuilder *_builder;

    public:
        Director(void);
        Director(IBuilder *builder);
        Director(Director &toCopy);
        Director &operator=(const Director &toCopy);
        
        void setBuilder(IBuilder *builder);
        void buildDefaultResponse(unsigned int resCode, const std::map<std::string, std::string> &resHeaders);

        ~Director();
    };
}

#endif