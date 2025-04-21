/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:49:31 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/21 21:49:31 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POSTUPLOAD_HPP
#define POSTUPLOAD_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <fstream>
#include <exception>

namespace Webserv
{
	class PostUpload
	{
	private:
		std::string _body;
		std::string _contentType;
		long _contentLength;
		std::string _accept;
		std::string obtainDelimiter(void);
		void processUpload(std::string &boundary);
		void extractMetadata(std::map<std::string, std::string> &headers, std::string &body);
		void downloadFile(std::map<std::string, std::string> &headers, std::string &body);

	public:
		PostUpload();
		PostUpload(const PostUpload &copy);
		PostUpload(std::string body, std::string contentType, long contentLength, std::string accept);
		PostUpload &operator=(const PostUpload &assign);
		void uploadFile(void);
		const std::string &getContentType(void) const;
		long getContentLength(void) const;
		const std::string &getAccept(void) const;
		class BodyParseError : std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
		class UploadError : std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
		~PostUpload();
	};
}

#endif