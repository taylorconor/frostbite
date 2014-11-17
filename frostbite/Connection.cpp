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

bool Connection::fileExists(std::string uri) {
    ifstream file;
    file.open(uri);
    if (!file)
        return 0;
    file.close();
    return 1;
}

// TODO: replace err with the HTTP status code
void Connection::printStatus(bool err) {
    if (err)
        cout << "ERR" << "\t";
    else
        cout << "OK" << "\t";
    
    cout << this->req.getRequestMethod() << "\t";
    cout << this->req.getRequestURI() << "\t";
    cout << endl;
}

void Connection::handleConnection() {
    std::string uri = getAbsoluteURI();
    std::string method = this->req.getRequestMethod();
    
    if (!fileExists(uri)) {
        printStatus(true);
        return;
    }
    
    // TODO: implement other HTTP methods
    if (method == "GET")
        this->res = Response(uri, this->sockfd);
    
    printStatus(false);
}

Connection::Connection() {}
Connection::Connection(Request req, int sockfd) {
    this->sockfd = sockfd;
    this->req = req;
}