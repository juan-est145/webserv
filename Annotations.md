In HTTP communications, determining whether you have received the entire message from the client using `recv` can be done by examining the HTTP headers and understanding the structure of the HTTP message. Here are the key methods to determine if the entire message has been received:

### 1. Content-Length Header
For HTTP/1.1 requests, the `Content-Length` header specifies the exact number of bytes in the body of the request. You can use this header to determine when you have received the entire body.

### 2. Chunked Transfer Encoding
For HTTP/1.1 requests, if the `Transfer-Encoding` header is set to `chunked`, the body is sent in chunks. Each chunk is preceded by its size in hexadecimal format, followed by `\r\n`. The end of the message is indicated by a chunk of size 0.

### 3. Connection Close
For HTTP/1.0 requests, the end of the message can be determined by the server closing the connection. This is less common in modern HTTP/1.1 communications.

### Example Code

Here is an example of how you might handle these cases in your server code:

```cpp
void Server::readSocket(struct epoll_event &eventList, struct epoll_event &eventConf)
{
    char buffer[1024];
    std::cout << "Reading from client " << eventList.data.fd << std::endl;
    ssize_t bufRead = recv(eventList.data.fd, buffer, sizeof(buffer), 0);
    if (bufRead <= 0)
    {
        eventConf.events = EPOLLIN;
        eventConf.data.fd = eventList.data.fd;
        if (bufRead == -1 || epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, eventList.data.fd, &eventConf) == -1)
        {
            close(this->_epollFd);
            Webserv::Logger::errorLog(errno, strerror, false);
            throw Server::ServerException();
        }
        close(eventList.data.fd);
        return;
    }

    Request *req = new Request(eventList.data.fd);
    req->processReq(buffer);

    // Check if the request has a body
    if (req->getMethod() == Request::POST)
    {
        std::string contentLengthStr = req->getReqHeader().at("Content-Length");
        if (!contentLengthStr.empty())
        {
            size_t contentLength = std::stoul(contentLengthStr);
            std::string body;
            body.reserve(contentLength);

            // Read the body
            while (body.size() < contentLength)
            {
                bufRead = recv(eventList.data.fd, buffer, sizeof(buffer), 0);
                if (bufRead <= 0)
                {
                    eventConf.events = EPOLLIN;
                    eventConf.data.fd = eventList.data.fd;
                    if (bufRead == -1 || epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, eventList.data.fd, &eventConf) == -1)
                    {
                        close(this->_epollFd);
                        Webserv::Logger::errorLog(errno, strerror, false);
                        throw Server::ServerException();
                    }
                    close(eventList.data.fd);
                    return;
                }
                body.append(buffer, bufRead);
            }

            req->setBody(body);
        }
        else if (req->getReqHeader().at("Transfer-Encoding") == "chunked")
        {
            std::string body;
            while (true)
            {
                // Read chunk size
                bufRead = recv(eventList.data.fd, buffer, sizeof(buffer), 0);
                if (bufRead <= 0)
                {
                    eventConf.events = EPOLLIN;
                    eventConf.data.fd = eventList.data.fd;
                    if (bufRead == -1 || epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, eventList.data.fd, &eventConf) == -1)
                    {
                        close(this->_epollFd);
                        Webserv::Logger::errorLog(errno, strerror, false);
                        throw Server::ServerException();
                    }
                    close(eventList.data.fd);
                    return;
                }
                std::string chunkSizeStr(buffer, bufRead);
                size_t chunkSize = std::stoul(chunkSizeStr, nullptr, 16);
                if (chunkSize == 0)
                {
                    break; // End of chunks
                }

                // Read chunk data
                while (chunkSize > 0)
                {
                    bufRead = recv(eventList.data.fd, buffer, std::min(sizeof(buffer), chunkSize), 0);
                    if (bufRead <= 0)
                    {
                        eventConf.events = EPOLLIN;
                        eventConf.data.fd = eventList.data.fd;
                        if (bufRead == -1 || epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, eventList.data.fd, &eventConf) == -1)
                        {
                            close(this->_epollFd);
                            Webserv::Logger::errorLog(errno, strerror, false);
                            throw Server::ServerException();
                        }
                        close(eventList.data.fd);
                        return;
                    }
                    body.append(buffer, bufRead);
                    chunkSize -= bufRead;
                }

                // Read the trailing CRLF
                bufRead = recv(eventList.data.fd, buffer, 2, 0);
                if (bufRead <= 0)
                {
                    eventConf.events = EPOLLIN;
                    eventConf.data.fd = eventList.data.fd;
                    if (bufRead == -1 || epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, eventList.data.fd, &eventConf) == -1)
                    {
                        close(this->_epollFd);
                        Webserv::Logger::errorLog(errno, strerror, false);
                        throw Server::ServerException();
                    }
                    close(eventList.data.fd);
                    return;
                }
            }

            req->setBody(body);
        }
    }

    this->_clientPool[eventList.data.fd] = req;
    if (!AuxFunc::handle_ctl(this->_epollFd, EPOLL_CTL_MOD, EPOLLOUT, eventList.data.fd, eventConf))
        throw Server::ServerException();
}
```

### Explanation

1. **Reading the Initial Data**: The initial call to `recv` reads the headers and possibly part of the body.
2. **Processing the Request**: The `Request` object processes the headers.
3. **Checking for a Body**: If the request method is `POST`, the `Content-Length` header is checked to determine the length of the body.
4. **Reading the Body**: A loop continues to call `recv` until the entire body has been read. The body is appended to a string until its size matches the `Content-Length`.
5. **Handling Chunked Transfer Encoding**: If the `Transfer-Encoding` header is set to `chunked`, the body is read in chunks until a chunk of size 0 is encountered.
6. **Storing the Request**: The `Request` object is stored in the `_clientPool` map.

By implementing these methods, you can ensure that your server correctly handles the entire message from the client, including the body, for both `Content-Length` and `chunked` transfer encoding.