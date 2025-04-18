/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juestrel <juestrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:15:36 by juestrel          #+#    #+#             */
/*   Updated: 2025/04/15 11:29:48 by juestrel         ###   ########.fr       */
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
    signal(SIGINT, &intSigHandler);
    try
    {
        Webserv::ConfigParser parser;
        Webserv::Cluster *cluster = NULL;
        parser.initConfigParser(confFile);
        cluster = Webserv::Cluster::cluster->getInstance(parser.getServers());
        cluster->initVirtualServers();
    }
    catch (const Webserv::Server::ServerException &e)
    {
        std::cerr << e.what() << std::endl;
        if (Webserv::Cluster::cluster != NULL)
            delete Webserv::Cluster::cluster;
        return (EXIT_FAILURE);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        if (Webserv::Cluster::cluster != NULL)
            delete Webserv::Cluster::cluster;
        return (EXIT_FAILURE);
    }
    if (Webserv::Cluster::cluster != NULL)
        delete Webserv::Cluster::cluster;
    return (EXIT_SUCCESS);
}