/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:56:55 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/13 23:57:25 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

#include <sys/stat.h>
#include "AuxFunc.hpp"
#include "IBuilder.hpp"
#include "Request.hpp"

namespace Webserv
{
    class Builder;

    class Director
    {
    private:
        IBuilder *builder;

    public:
        Director(void);
        Director(IBuilder *builder);
        Director(Director &toCopy);
        Director &operator=(const Director &other);
        ~Director();
        void SetBuilder(IBuilder *builder);
        void BuildDefaultResponse(const Request *rq); // CODE 200
    };
}

#endif