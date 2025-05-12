/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/09 14:51:35 by mfuente-         ###   ########.fr       */
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
		builder->SetStatus(AuxFunc::ft_itoa(rq->getResCode()));
		builder->SetContent(rq->getResourceMime());
		builder->SetContentLength(rq->getResourceSize());
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
	// CODE 301 & 401 POSIBLE ELIMINAR
	void Director::BuildLocationResponse(const Request *rq)
	{
		builder->SetStatus(AuxFunc::ft_itoa(rq->getResCode()));
		builder->SetContent(rq->getResourceMime());
		builder->SetContentLength(rq->getResourceSize());
		
	}
	// CODE 404 POSIBLE ELIMINAR
	void Director::BuildErrorResponse(const Request *rq)
	{
		this->builder->SetStatus(AuxFunc::ft_itoa(rq->getResCode()));
		this->builder->SetContent(rq->getResourceMime());
		builder->SetContentLength(rq->getResourceSize());
	}
}