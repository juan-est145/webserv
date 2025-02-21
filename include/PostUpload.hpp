#ifndef POSTUPLOAD_HPP
#define POSTUPLOAD_HPP

#include <iostream>
#include <string>

namespace Webserv
{
	class PostUpload
	{
	private:
		std::string _contentType;
		long _contentLength;
		std::string _accept;

	public:
		PostUpload();
		PostUpload(const PostUpload &copy);
		PostUpload(std::string contentType, long contentLength, std::string accept);
		PostUpload &operator=(const PostUpload &assign);
		const std::string &getContentType(void) const;
		long getContentLength(void) const;
		const std::string &getAccept(void) const;
		~PostUpload();
	};
}

#endif