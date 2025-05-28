/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 15:24:44 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/28 21:52:04 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_H
#define DELETE_H

#include <cstdio>
#include "AServerAction.hpp"
#include "Cookie.hpp"

namespace Webserv
{
    class Delete : public AServerAction
    {
    public:
        Delete();
        Delete(const std::string path);
        Delete(const Delete &copy);
        Delete &operator=(const Delete &assign);
        ~Delete();

        void deleteFile(const std::string &localPath);
        void processRequest(
            const ConfigServer *config,
            struct RequestData &reqData,
            const std::map<std::string, Webserv::CookieData> &sessions);
        void mainAction(const ConfigServer *config, struct RequestData &reqData);
        void redirect(const std::string &uri, const ConfigServer *config);
    };
}
#endif