/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 21:49:31 by juestrel          #+#    #+#             */
/*   Updated: 2025/05/20 08:23:45 by juestrel         ###   ########.fr       */
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
#include "AServerAction.hpp"
#include "Request.hpp"

namespace Webserv
{
	class Request;

	class PostUpload : public AServerAction
	{
	private:
		std::string _body;
		std::string _contentType;
		long _contentLength;
		std::string _accept;

		void findHeaders(const Request &req);
		void checkValidHeader(std::map<std::string, std::string>::const_iterator &it, const std::map<std::string, std::string> &reqHeaders);
		void uploadFile(const std::string &directory);
		std::string obtainDelimiter(void);
		void processUpload(std::string &boundary, const std::string &localPath);
		void extractMetadata(std::map<std::string, std::string> &headers, std::string &body);
		void downloadFile(std::map<std::string, std::string> &headers, std::string &body, const std::string &localPath);
		void createBodyMessage(void);
		void addLocationMember(const ConfigServer *config);
		void redirect(const std::string &uri, const ConfigServer *config);

	public:
		PostUpload();
		PostUpload(const PostUpload &copy);
		PostUpload(std::string body, const std::string path);
		PostUpload &operator=(const PostUpload &assign);

		void processRequest(
			const ConfigServer *config,
			const Request &req,
			const std::map<std::string, Webserv::CookieData> &sessions);

		const std::string &getContentType(void) const;
		long getContentLength(void) const;
		const std::string &getAccept(void) const;

		~PostUpload();
	};
}

#endif