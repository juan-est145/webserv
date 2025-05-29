/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiReq.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:33:31 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/29 23:24:05 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CgiReq.hpp"

namespace Webserv
{
    CgiReq::CgiReq(void) : ARequest()
    {
        this->_pipeFd[PIPE_READ] = -1;
        this->_pipeFd[PIPE_WRITE] = -1;
        this->_ogReqSock = -1;
        this->_childPid = 0;
    }

    CgiReq::CgiReq(int *pipeFd, int _ogReqSock, pid_t childPid) : ARequest(pipeFd[PIPE_READ])
    {
        this->_pipeFd[PIPE_READ] = pipeFd[PIPE_READ];
        this->_pipeFd[PIPE_WRITE] = pipeFd[PIPE_WRITE];
        this->_ogReqSock = _ogReqSock;
        this->_childPid = childPid;
    }

    CgiReq::CgiReq(const CgiReq &toCopy) : ARequest(toCopy)
    {
        *this = toCopy;
    }

    CgiReq &CgiReq::operator=(const CgiReq &toCopy)
    {
        if (this != &toCopy)
        {
            this->_pipeFd[PIPE_READ] = toCopy._pipeFd[PIPE_READ];
            this->_pipeFd[PIPE_WRITE] = toCopy._pipeFd[PIPE_WRITE];
            this->_ogReqSock = toCopy._ogReqSock;
            this->_childPid = toCopy._ogReqSock;
        }
        return (*this);
    }

    int CgiReq::getOgReqSock(void) const
    {
        return (this->_ogReqSock);
    }

    pid_t CgiReq::getChildPid(void) const
    {
        return (this->_childPid);
    }

    CgiReq::~CgiReq() {}
}