/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:56:55 by mfuente-          #+#    #+#             */
/*   Updated: 2025/04/22 13:43:43 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

#include <sys/stat.h>
#include "AuxFunc.hpp"
#include "Builder.hpp"

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
        void BuildOkResponse(long size); // CODE 200
        void BuildOkUploadResponse(long size);   // CODE 201
        void BuildErrorResponse(long size, unsigned int resCode); //Error respose
    };
}

#endif