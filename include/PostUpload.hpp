#ifndef POSTUPLOAD_HPP
#define POSTUPLOAD_HPP

#include <iostream>
#include <string>

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

	public:
		PostUpload();
		PostUpload(const PostUpload &copy);
		PostUpload(std::string body, std::string contentType, long contentLength, std::string accept);
		PostUpload &operator=(const PostUpload &assign);
		void uploadFile(void);
		const std::string &getContentType(void) const;
		long getContentLength(void) const;
		const std::string &getAccept(void) const;
		~PostUpload();
	};
}

#endif