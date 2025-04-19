/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerAction.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:51:54 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/19 17:53:49 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERACTION_HPP
#define SERVERACTION_HPP

#include <iostream>
#include <string>

namespace Webserv
{
	class ServerAction
	{
	protected:
		/* data */
	public:
		ServerAction(void);
		virtual ~ServerAction();
	};
}
#endif