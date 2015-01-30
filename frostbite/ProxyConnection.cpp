//
//  ProxyConnection.cpp
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#include "ProxyConnection.h"

void ProxyConnection::handleConnection() {
    if (!this->req->isValid()) {
        ERR_RESPONSE;
        return;
    }
    
    cout << "Proxy " << req->getRequestMethod() << " " <<
        req->getRequestURI() << endl;
    
    std::string host = req->getRequestParam("Host");
    ssize_t n;
    int srv_sockfd = Socket::openSocket(host, 80);
    fcntl(srv_sockfd, F_SETFL, O_NONBLOCK);
    
    struct timeval tv;
    fd_set readfds;
    
    tv.tv_sec = 2;
    tv.tv_usec = 500000;
    
    FD_ZERO(&readfds);
    FD_SET(srv_sockfd, &readfds);
    
    char buf[MAX_BUF];
    
    std::string data = req->getSource();
    n = write(srv_sockfd, data.c_str(), strlen(data.c_str()));
    if (n < 0)
        Utils::error("ERROR sending data");
    
    select(srv_sockfd+1, &readfds, NULL, NULL, &tv);
    
    while ((n = read(srv_sockfd, buf, MAX_BUF)) > 0) {
        buf[n] = '\0';
        n = write(sockfd, buf, n);
        if (n < 0)
            Utils::error("ERROR sending data");
        select(srv_sockfd+1, &readfds, NULL, NULL, &tv);
    }
    perror("ERROR on read");
    
    close(srv_sockfd);
    cout << "Done  " << req->getRequestMethod() << " " <<
        req->getRequestURI() << endl;
    this->completed = true;
}

ProxyConnection::ProxyConnection(Request *req, int sockfd) {
    this->req = req;
    this->res = nullptr;
    this->sockfd = sockfd;
    this->completed = false;
}