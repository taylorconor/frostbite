//
//  ProxyConnection.cpp
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#include "ProxyConnection.h"

void ProxyConnection::print_status() {
    mtx->lock();
    std::cout << "PROXY " << req->uri() << std::endl;
    mtx->unlock();
}

void ProxyConnection::handle_connection() {
    if (!this->req->is_valid()) {
        ERR_RESPONSE;
        return;
    }
    
    if (req->method().compare("GET") == 0) {
        int proxy_sockfd = Socket::create(req->host(), req->port());
        if (proxy_sockfd != -1) {
            // send the request to the destination server
            send(proxy_sockfd, req->source().c_str(), req->source().length(),0);
            
            // set a 5-second timeout on recv
            struct timeval tv;
            tv.tv_sec = 2;
            tv.tv_usec = 0;
            setsockopt(proxy_sockfd, SOL_SOCKET, SO_RCVTIMEO,
                       (char *)&tv,sizeof(struct timeval));
            
            // receive the destination server's response, and send that back to
            // the client
            ssize_t n;
            while ((n = recv(proxy_sockfd, recbuf, MAX_BUF, 0)) > 0) {
                send(_sockfd, recbuf, n, 0);
            }
            print_status();
        }
    }
    
    this->completed = true;
}

ProxyConnection::ProxyConnection(Request *req, int sockfd) {
    this->req = req;
    this->res = nullptr;
    this->_sockfd = sockfd;
    this->completed = false;
    this->recbuf = new char[MAX_BUF];
}