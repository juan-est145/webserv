/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:33:31 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/27 18:48:48 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CgiReq.hpp"

namespace Webserv
{
    CgiReq::CgiReq(void) : _ogReq(NULL)
    {
        this->_pipeFd[PIPE_READ] = -1;
        this->_pipeFd[PIPE_WRITE] = -1;
    }

    CgiReq::CgiReq(int *pipeFd, Request *ogReq) : _ogReq(ogReq)
    {
        this->_pipeFd[PIPE_READ] = pipeFd[PIPE_READ];
        this->_pipeFd[PIPE_WRITE] = pipeFd[PIPE_WRITE];
    }

    CgiReq::CgiReq(const CgiReq &toCopy) : _ogReq(toCopy._ogReq)
    {
        *this = toCopy;
    }

    CgiReq &CgiReq::operator=(const CgiReq &toCopy)
    {
        if (this != &toCopy)
        {
            this->_pipeFd[PIPE_READ] = toCopy._pipeFd[PIPE_READ];
            this->_pipeFd[PIPE_WRITE] = toCopy._pipeFd[PIPE_WRITE];
        }
        return (*this);
    }

    void CgiReq::readReq(const char *buffer, size_t bufSize)
    {
       std::string strBuff(buffer, bufSize);
       this->_ogReq->setResourceContent(strBuff);
    }

    CgiReq::~CgiReq() {}
}