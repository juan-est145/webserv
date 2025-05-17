/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/17 18:07:34 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Director.hpp"

namespace Webserv
{
	Director::Director(void)
	{
		this->_builder = NULL;
	}
	Director::Director(IBuilder *builder)
	{
		this->_builder = builder;
	}

	Director::Director(Director &toCopy)
	{
		*this = toCopy;
	}

	Director &Director::operator=(const Director &toCopy)
	{
		if (this != &toCopy)
			this->_builder = toCopy._builder;
		return (*this);
	}
	Director::~Director()
	{
	}
	void Director::SetBuilder(IBuilder *builder)
	{
		this->_builder = builder;
	}
	// DEFAULT
	void Director::BuildDefaultResponse(const Request *rq)
	{
		std::string dateTime = AuxFunc::getGmtTime();

		this->_builder->setResCode(AuxFunc::ft_itoa(rq->getResCode()));
		// this->_builder->setMime(rq->getResourceMime());
		// this->_builder->setContentLength(rq->getResourceSize());
		this->_builder->setDate(dateTime);
		this->_builder->setHeaders(rq->getResHeaders());
		// if (rq->getResCode() == 201 || rq->getResCode() == 301 || rq->getResCode() == 302)
		// 	this->_builder->setLocation(rq->getLocation());
		// else if (rq->getResCode() == 405)
        //     this->_builder->setAllow(rq->getAllow());

		/*
		if (req->getResCode() == 301 || req->getResCode() == 302)
			builder->setLocation(rq->getLocation());
	    else if (req->getResCode() == 401)
            builder->setWwwAuthenticate(rq->getWwwAuthenticate()); 
	    else if (req->getResCode() == 405)
            builder->setAllow(rq->getAllow()); 
	    else if (req->getResCode() == 503)
            this->setRetryAfter(rq->getRetryAfter()); 

		*/
	}
}