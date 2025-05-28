/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiReq.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:26:11 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/28 19:58:45 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIREQ_HPP
#define CGIREQ_HPP

#include <string>
#include <iostream>
#include "ARequest.hpp"
#include "Request.hpp"

namespace Webserv
{
    class Request;

    class CgiReq : public ARequest
    {
    private:
        Request *_ogReq;
        int _pipeFd[2];

    public:
        CgiReq(void);
        CgiReq(int *pipeFd, Request *ogReq);
        CgiReq(const CgiReq &toCopy);
        CgiReq &operator=(const CgiReq &toCopy);

        void readReq(const char *buffer, size_t bufSize);
        
        const Request *getOgReq(void) const;
        
        std::size_t setResourceContent(const std::string &content);
        
        ~CgiReq();
    };
}

#endif