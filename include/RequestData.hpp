/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestData.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 21:18:19 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/28 21:21:55 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTDATA_HPP
#define REQUESTDATA_HPP

#include <string>
#include <map>
#include "FirstHeader.hpp"

struct RequestData
{
	struct firstHeader _firstHeader;
	std::map<std::string, std::string> _reqHeader;
	std::string _reqBody;
};

#endif