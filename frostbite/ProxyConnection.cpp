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
    
    if (req->getRequestMethod().compare("GET") == 0) {
        
        int proxy_sockfd = Socket::openSocket(req->getRequestParam("Host"), 80);
        if (proxy_sockfd != -1) {
            std::cout << req->getRequestMethod() << " " << req->getRequestURI()
                << " " << req->getRequestHTTP() << std::endl;
            
            // send the request to the destination server
            send(proxy_sockfd, req->getSource().c_str(),
                 req->getSource().length(), 0);
            
            struct timeval tv;
            tv.tv_sec = 5;
            tv.tv_usec = 0;
            setsockopt(proxy_sockfd, SOL_SOCKET, SO_RCVTIMEO,
                       (char *)&tv,sizeof(struct timeval));
            
            // receive the destination server's response, and send that back to
            // the client
            ssize_t n;
            while ((n = recv(proxy_sockfd, recbuf, MAX_BUF, 0)) > 0) {
                send(sockfd, recbuf, n, 0);
            }
        }
    }
    
    this->completed = true;
}

ProxyConnection::ProxyConnection(Request *req, int sockfd) {
    this->req = req;
    this->res = nullptr;
    this->sockfd = sockfd;
    this->completed = false;
    this->recbuf = new char[MAX_BUF];
}