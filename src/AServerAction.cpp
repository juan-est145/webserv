/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AServerAction.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:05:50 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/20 14:35:31 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/AServerAction.hpp"

namespace Webserv
{
	AServerAction::AServerAction(void): _path("")
	{
		this->_content = "";
		this->_size = -1;
	}

	AServerAction::AServerAction(const std::string path): _path(path)
	{
		this->_content = "";
		this->_size = -1;
	}

	AServerAction::AServerAction(const AServerAction &toCopy)
	{
		*this = toCopy;
	}

	AServerAction &AServerAction::operator=(const AServerAction &toCopy)
	{
		if (this != &toCopy)
		{
			this->_content = toCopy._content;
			this->_size = toCopy._size;
		}
		return (*this);
	}

	const std::string &AServerAction::getPath(void) const
	{
		return (this->_path);
	}

	const std::string &AServerAction::getContent(void) const
	{
		return (this->_content);
	}

	long AServerAction::getSize(void) const
	{
		return (this->_size);
	}

	unsigned int AServerAction::getResCode(void) const
	{
		return (this->_resCode);
	}

	void AServerAction::setRescode(unsigned int resCode)
	{
		this->_resCode = resCode;
	}

	AServerAction::~AServerAction() {}
}