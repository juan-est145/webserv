/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:36 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/12 17:46:28 by juestrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

// TO DO: Check if port number is under 1024 and the bind process fails, send a message that says that with that port number, the server must run with root privileges.
// 65535 is the maximum port number

bool g_stop = false;

void intSigHandler(int signum)
{
    if (signum == SIGINT)
        g_stop = true;
}

int main(int argc, char *argv[])
{
    std::string confFile = argc < 2 ? "./config/default.conf" : argv[1];
    std::string host = "127.0.0.1";
    std::string port = "3000";
    Webserv::Server *server = NULL;
    signal(SIGINT, &intSigHandler);
    try
    {
        Webserv::ConfigParser parser;
        Webserv::Cluster *cluster = NULL;
        parser.initConfigParser(confFile);
        cluster = Webserv::Cluster::cluster->getInstance(parser.getServers());
        cluster->initVirtualServers();
        // TO DO: When cluster has been fully implemented, delete the old methods like creating an standalone server
        server = new Webserv::Server(host, port);
        server->initServer();
    }
    catch (const Webserv::Server::ServerException &e)
    {
        std::cerr << e.what() << std::endl;
        if (server)
            delete server;
        if (Webserv::Cluster::cluster != NULL)
            delete Webserv::Cluster::cluster;
        return (EXIT_FAILURE);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        if (server)
            delete server;
        if (Webserv::Cluster::cluster != NULL)
            delete Webserv::Cluster::cluster;
        return (EXIT_FAILURE);
    }
    delete server;
    if (Webserv::Cluster::cluster != NULL)
        delete Webserv::Cluster::cluster;
    return (EXIT_SUCCESS);
}