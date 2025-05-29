/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiReq.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:26:11 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/29 12:34:28 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIREQ_HPP
#define CGIREQ_HPP

#ifndef PIPE_READ
#define PIPE_READ 0
#endif

#ifndef PIPE_WRITE
#define PIPE_WRITE 1
#endif

#include <string>
#include <iostream>
#include "ARequest.hpp"

namespace Webserv
{

    class CgiReq : public ARequest
    {
    private:
        int _ogReqSock;
        int _pipeFd[2];

    public:
        CgiReq(void);
        CgiReq(int *pipeFd, int _ogReqSock);
        CgiReq(const CgiReq &toCopy);
        CgiReq &operator=(const CgiReq &toCopy);
        
        int getOgReqSock(void) const;
                
        ~CgiReq();
    };
}

#endif