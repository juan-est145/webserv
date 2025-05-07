/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/07 18:14:18 by mfuente-         ###   ########.fr       */
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
	// CODE 200
	void Director::BuildOkResponse(const Request *rq)
	{
		builder->SetStatus(AuxFunc::ft_itoa(rq->getResCode()));
		builder->SetContent(rq->getResourceMime());
		builder->SetContentLength(rq->getResourceSize());
	}
	// CODE 201
	void Director::BuildOkUploadResponse(const Request *rq)
	{
		builder->SetStatus(AuxFunc::ft_itoa(rq->getResCode()));
		builder->SetContent(rq->getResourceMime());
		builder->SetContentLength(rq->getResourceSize());
	}
	// CODE 404
	void Director::BuildErrorResponse(const Request *rq)
	{
		this->builder->SetStatus(AuxFunc::ft_itoa(rq->getResCode()));
		this->builder->SetContent(rq->getResourceMime());
		builder->SetContentLength(rq->getResourceSize());
	}
}