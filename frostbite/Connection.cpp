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

#define ERR_RESPONSE    this->res = new Response(this->sockfd); \
                        this->res->send(HTTP_500_INTERNAL_ERR); \

std::vector<std::string> defaultFiles = {"index.html", "index.htm"};

std::string Connection::getAbsoluteURI() {
    string uri = "/Users/Conor/Documents/Projects/frostbite/srv/"
        +this->req->getRequestURI();
    
    struct stat s;
    if (stat(uri.c_str(), &s) == 0) {
        if(s.st_mode & S_IFDIR) {
            // it's a directory
            // add a slash to enter the directory if it's not present
            if (uri.back() != '/')
                uri += '/';
            
            for (int i = 0; i < defaultFiles.size(); i++) {
                if (Utils::exists(uri+defaultFiles[i])) {
                    uri += defaultFiles[i];
                    break;
                }
            }
        }
        else if(s.st_mode & S_IFREG) {
            // it's a file, uri is already the absolute uri
        }
        else {
            // internal server error
            return nullptr;
        }
    }
    
    return uri;
}

void Connection::printStatus() {
    int code = this->res->getCode();
    
    if (code > 400)
        cout << "ERR" << "\t";
    else
        cout << "OK" << "\t";
    
    cout << code << "\t";
    cout << this->req->getRequestMethod() << "\t";
    cout << this->req->getRequestURI() << "\t";
    cout << endl;
}

void Connection::handleConnection() {
    if (!this->req->isValid()) {
        ERR_RESPONSE;
        return;
    }
    std::string uri = getAbsoluteURI();
    if (uri.empty()) {
        ERR_RESPONSE;
        return;
    }
    std::string method = this->req->getRequestMethod();
    
    // TODO: implement other HTTP methods
    if (method == "GET") {
        this->res = new Response(uri, this->sockfd);
        this->res->send();
    }
    else {
        ERR_RESPONSE;
        return;
    }
    
    printStatus();
    delete this->res;
}

Connection::Connection() {}
Connection::Connection(Request *req, int sockfd) {
    this->sockfd = sockfd;
    this->req = req;
}