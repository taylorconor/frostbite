//
//  Server.cpp
//  frostbite
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Server.h"

// dispatch the request to the correct host for handling
void Server::dispatch(Request *req, int newsockfd) {
    std::string reqURI = req->getRequestURI();
    for (int i = 0; i < hosts.size(); i++) {
        Hostname *h = hosts[i]->getHostname();
        if (h->contains(reqURI)) {
            hosts[i]->handleRequest(req, newsockfd);
        }
    }
}

void Server::initListen(int sockfd) {
    int newsockfd;
    long n;
    char buffer[RECBUF];
    socklen_t clilen;
    sockaddr_in cli_addr;
    
    ::listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            Utils::error("ERROR on accept");
        bzero(buffer,256);
        n = read(newsockfd,buffer,RECBUF-1);
        if (n < 0)
            Utils::error("ERROR reading from socket");
        else if (n == RECBUF-1)
            printf("\n\n\nBUFFER OVERFLOW\n\n\n");
        
        Request *req = new Request(string(buffer));
        dispatch(req, newsockfd);
        
        close(newsockfd);
    }
}

void Server::initServer() {
    // init the recieving socket and server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        Utils::error("ERROR opening socket");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        Utils::error("ERROR opening socket");

    // set the server configs
    sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    // try to bind to the port
    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        Utils::error("ERROR on binding");
    
    // initialise server hosts
    // TODO: read hosts from config file
    std::vector<std::string> h { "localhost" };
    Hostname *hs = new Hostname(h);
    hosts.push_back(new Host(hs));
    
    // begin server listening
    initListen(sockfd);
    
    // close the server socket after listening is complete
    close(sockfd);
}

void Server::listen(int port) {
    this->port = port;
    initServer();
}

Server::Server() {}
Server::Server(int port) {
    this->port = port;
}