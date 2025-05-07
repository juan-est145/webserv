/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FirstHeader.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 16:05:55 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/07 16:08:02 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <utility>

enum method
{
	GET = 0,
	POST = 1,
	DELETE = 2,
	UNKNOWN = 3,
};

struct firstHeader
{
	std::string path;
	std::string httpVers;
	std::pair<std::string, enum method> method;
	std::string query;
};