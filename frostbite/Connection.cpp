//
//  Connection.cpp
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//
//  Represents a single connection to the server
//

#include "Connection.h"

std::string Connection::getAbsoluteURI() {
    string uri = "/Users/Conor/Documents/Projects/frostbite/srv"
    +this->req.getRequestURI();
    
    if (uri.back() == '/')
        uri += "index.html";
    
    return uri;
}

void Connection::printStatus() {
    int code = this->res.getCode();
    
    if (code > 400)
        cout << "ERR" << "\t";
    else
        cout << "OK" << "\t";
    
    cout << code << "\t";
    cout << this->req.getRequestMethod() << "\t";
    cout << this->req.getRequestURI() << "\t";
    cout << endl;
}

void Connection::handleConnection() {
    std::string uri = getAbsoluteURI();
    std::string method = this->req.getRequestMethod();
    
    // TODO: implement other HTTP methods
    if (method == "GET")
        this->res = Response(uri, this->sockfd);
    
    printStatus();
}

Connection::Connection() {}
Connection::Connection(Request req, int sockfd) {
    this->sockfd = sockfd;
    this->req = req;
}