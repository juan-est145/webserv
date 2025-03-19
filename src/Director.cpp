/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/03/19 18:59:43 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Director.hpp"

namespace Webserv
{
    Director::Director(void)
    {
        this->builder = NULL;
    }
    Director::Director(Builder *builder)
    {
        this->builder = builder;
    }
    Director &Director::operator=(const Director& other)
    {
        this->builder = other.builder;
        return *this;
    }
}




/* Director(void);
Director(Builder *builder);
Director(Director &toCopy);
Director &operator=(const Director& other);
~Director();
void SetBuilder(Builder *builder);
void BuildOkResponse();
void BuildOkUploadResponse();        
void BuildNotFoundResponse(); */