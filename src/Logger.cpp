/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:30 by juestrel          #+#    #+#             */
/*   Updated: 2025/02/08 12:15:30 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Logger.hpp"

namespace Webserv
{
	Logger::Logger(void) {}

	Logger::Logger(Logger &toCopy)
	{
		*this = toCopy;
	}

	Logger &Logger::operator=(const Logger &assign)
	{
		(void)assign;
		return *this;
	}

	void Logger::errorLog(const int errorCode, const char*(&f)(int), const bool fatal)
	{
		std::string error(f(errorCode));
		std::cerr << "Error " << error << std::endl;
		if (fatal)
			exit(EXIT_FAILURE);
	}

	void Logger::errorLog(const int errorCode, char*(&f)(int), const bool fatal)
	{
		std::string error(f(errorCode));
		std::cerr << "Error " << error << std::endl;
		if (fatal)
			exit(EXIT_FAILURE);
	}

	Logger::~Logger(void) {}

}