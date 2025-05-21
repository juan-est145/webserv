/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:24:44 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/21 15:51:36 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_H
#define DELETE_H

#include "AServerAction.hpp"
#include "Request.hpp"
#include <cstdio>
namespace Webserv
{
    class Delete : public AServerAction
    {
        private:
            
        public:
            Delete();
            Delete(const std::string path);
            Delete(const Delete &copy);
            Delete &operator=(const Delete &assign);
            ~Delete();

            void deleteArchive(std::string archToDelete);
            void processRequest(
            const ConfigServer *config, 
            const Request &req, 
            const std::map<std::string, Webserv::CookieData> &sessions);
            void obtainResource(const ConfigServer *config, const Request &req);
    };
}
#endif