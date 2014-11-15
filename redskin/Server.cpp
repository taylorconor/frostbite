//
//  Server.cpp
//  redskin
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Server.h"

void Server::initListen(int sockfd) {
    int newsockfd, n;
    char buffer[RECBUF];
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    
    listen(sockfd,5);
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
        
        Request req = Request(string(buffer));
        Response res = Response(req, newsockfd);
        
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
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    // try to bind to the port
    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        Utils::error("ERROR on binding");
    
    // begin server listening
    initListen(sockfd);
    
    // close the server socket after listening is complete
    close(sockfd);
}

Server::Server(int port) {
    this->port = port;
    initServer();
}