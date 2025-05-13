/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:56:55 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/13 12:53:56 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

#include <sys/stat.h>
#include "AuxFunc.hpp"
#include "Builder.hpp"
#include "Request.hpp"

namespace Webserv
{
    class Builder;

    class Director
    {
    private:
        Builder *builder;

    public:
        Director(void);
        Director(Builder *builder);
        Director(Director &toCopy);
        Director &operator=(const Director &other);
        ~Director();
        void SetBuilder(Builder *builder);
        void BuildDefaultResponse(const Request *rq); // CODE 200
    };
}

#endif