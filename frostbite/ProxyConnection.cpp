//
//  ProxyConnection.cpp
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#include "ProxyConnection.h"

void ProxyConnection::print_status() {
    std::string output = "PROXY ";
    switch (status) {
        case PSTATUS_OK:
            output += "OK\t";
            break;
        case PSTATUS_METHOD_NOT_SUPPORTED:
            output += "MNS\t";
            break;
        case PSTATUS_SOCK_INIT_FAIL:
            output += "SIF\t";
            break;
        case PSTATUS_SEND_FAIL:
            output += "SF\t";
            break;
        case PSTATUS_INTERNAL_ERROR:
        default:
            output += "ERR\t";
            break;
    }
    std::string uri = req->uri();
    if (uri.length() > OUTPUT_STR_LEN)
        uri = uri.substr(0,OUTPUT_STR_LEN) + "...";
    output += uri;
    
    mtx->lock();
    std::cout << output << std::endl;
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
            ssize_t n, m;
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
            while ((n = recv(proxy_sockfd, recbuf, MAX_BUF, 0)) > 0) {
                m = send(_sockfd, recbuf, n, 0);
                if (m != n) {
                    status = PSTATUS_SEND_FAIL;
                    break;
                }
            }
            status = PSTATUS_OK;
        }
        else {
            status = PSTATUS_SOCK_INIT_FAIL;
        }
    }
    else {
        status = PSTATUS_METHOD_NOT_SUPPORTED;
    }
    
    this->completed = true;
}

ProxyConnection::ProxyConnection(Request *req, int sockfd) {
    this->req = req;
    this->res = nullptr;
    this->_sockfd = sockfd;
    this->completed = false;
    this->recbuf = new char[MAX_BUF];
    this->status = PSTATUS_INTERNAL_ERROR;
}