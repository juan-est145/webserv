/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/14 11:01:15 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Director.hpp"

namespace Webserv
{
	Director::Director(void)
	{
		this->builder = NULL;
	}
	Director::Director(IBuilder *builder)
	{
		this->builder = builder;
	}

	Director::Director(Director &toCopy)
	{
		*this = toCopy;
	}

	Director &Director::operator=(const Director &toCopy)
	{
		if (this != &toCopy)
			this->builder = toCopy.builder;
		return (*this);
	}
	Director::~Director()
	{
	}
	void Director::SetBuilder(IBuilder *builder)
	{
		this->builder = builder;
	}
	// DEFAULT
	void Director::BuildDefaultResponse(const Request *rq)
	{
		this->builder->setResCode(AuxFunc::ft_itoa(rq->getResCode()));
		this->builder->setMime(rq->getResourceMime());
		this->builder->setContentLength(rq->getResourceSize());
		if (rq->getResCode() == 201 || rq->getResCode() == 301 || rq->getResCode() == 302)
			this->builder->setLocation(rq->getLocation());

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