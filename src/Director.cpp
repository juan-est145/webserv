/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/05 14:43:47 by mfuente-         ###   ########.fr       */
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
	void Director::BuildOkResponse(long size, std::string mime)
	{
		builder->SetStatus("200");
		builder->SetContent(mime);
		builder->SetContentLength(size);
	}
	// CODE 201
	void Director::BuildOkUploadResponse(long size, std::string mime)
	{
		this->builder->SetStatus("201");
		this->builder->SetContent(mime);
		this->builder->SetContentLength(size);
	}
	// CODE 404
	void Director::BuildErrorResponse(long size, unsigned int resCode, std::string mime)
	{
		if (resCode < 400)
			throw std::exception();
		this->builder->SetStatus(AuxFunc::ft_itoa(resCode));
		this->builder->SetContent(mime);
		this->builder->SetContentLength(size);
	}
}