/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:56:55 by mfuente-          #+#    #+#             */
/*   Updated: 2025/04/11 12:18:03 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

#include "Builder.hpp"
#include <sys/stat.h>

namespace Webserv
{
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
        void BuildOkResponse(const char *path); // CODE 200
        void BuildOkUploadResponse(int size);   // CODE 201
        void BuildNotFoundResponse();           // CODE 404
    };
    int getFileSize(const char *filename);

}

#endif