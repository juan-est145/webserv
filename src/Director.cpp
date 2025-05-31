/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Director.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:48:38 by mfuente-          #+#    #+#             */
/*   Updated: 2025/05/31 12:54:11 by juestrel         ###   ########.fr       */
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
	void Director::setBuilder(IBuilder *builder)
	{
		this->_builder = builder;
	}

	void Director::buildDefaultResponse(unsigned int resCode, const std::map<std::string, std::string> &resHeaders)
	{
		std::string dateTime = AuxFunc::getGmtTime();

		this->_builder->setResCode(AuxFunc::ft_itoa(resCode));
		this->_builder->setDate(dateTime);
		this->_builder->setHeaders(resHeaders);
	}
}