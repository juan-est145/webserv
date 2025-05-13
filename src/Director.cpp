/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/13 18:18:57 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Director.hpp"

namespace Webserv
{
	Director::Director(void)
	{
		this->builder = NULL;
	}
	Director::Director(Builder *builder)
	{
		this->builder = builder;
	}

	Director::Director(Director &toCopy)
	{
		*this = toCopy;
	}

	Director &Director::operator=(const Director &other)
	{
		this->builder = other.builder;
		return *this;
	}
	Director::~Director()
	{
	}
	void Director::SetBuilder(Builder *builder)
	{
		this->builder = builder;
	}
	// DEFAULT
	void Director::BuildDefaultResponse(const Request *rq)
	{
		this->builder->SetStatus(AuxFunc::ft_itoa(rq->getResCode()));
		this->builder->SetContent(rq->getResourceMime());
		this->builder->SetContentLength(rq->getResourceSize());
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