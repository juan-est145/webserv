/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:44:06 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/20 17:31:08 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Delete.hpp"

namespace Webserv
{
    Delete::Delete()
    {
        archive = "";
    }
    Delete::Delete(const Delete &copy)
    {
        *this = copy;
    }
    Delete &Delete::operator=(const Delete &assign)
    {
        if (this != &assign)
        {
            this->archive = assign.archive;
        }
        return(*this);
    }
    void Delete::deleteArchive(std::string archToDelete)
    {
        int r = std::remove(archToDelete.c_str());
        int result;
        if(r == 0)
        result = 200;
        else
        result = 404;
        this->setRescode(result);
    }
}