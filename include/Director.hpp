/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:56:55 by mfuente-          #+#    #+#             */
/*   Updated: 2025/04/19 14:23:05 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

#include <sys/stat.h>
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
        void BuildNotFoundResponse();           // CODE 404
    };
    int getFileSize(const char *filename);

}

#endif