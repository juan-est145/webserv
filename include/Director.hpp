/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:56:55 by mfuente-          #+#    #+#             */
/*   Updated: 2025/03/19 18:34:11 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

#include "Builder.hpp"

namespace Webserv
{
    class Director{
        private:
            Builder *builder;
        public:
            Director(void);
            Director(Builder *builder);
            Director(Director &toCopy);
            Director &operator=(const Director& other);
            ~Director();
            void SetBuilder(Builder *builder);
            void BuildOkResponse();
            void BuildOkUploadResponse();        
            void BuildNotFoundResponse();
        
    };
}



#endif