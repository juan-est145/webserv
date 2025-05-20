/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:24:44 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/20 12:50:05 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_H
#define DELETE_H

#include "AServerAction.hpp"
#include <cstdio>
namespace Webserv
{
    class Delete : public AServerAction
    {
        private:
            std::string archive; 
        public:
            Delete();
            Delete(const Delete &copy);
            Delete &operator=(const Delete &assign);
            ~Delete();

            void deleteArchive(std::string archToDelete);
            
    };
}
#endif