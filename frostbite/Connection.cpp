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

abs_uri *Connection::getAbsoluteURI() {
    abs_uri *res = new abs_uri;
    
    std::string r = this->req->getRequestURI();
    if (r.front() == '/')
        r.erase(0,1);
    std::string uri = this->location + r;
    
    struct stat s;
    if (stat(uri.c_str(), &s) == 0) {
        if(s.st_mode & S_IFDIR) {
            // it's a directory
            
            // if there's no trailing slash then the browser is going to get
            // really confused. we need to responde with a 301 Moved Permanently
            // to tell the browser that the real location has a trailing slash
            if (uri.back() != '/') {
                res->status = HTTP_301_MOVED;
                // here we'll add the slash so that Response will know what to
                // tell the browser to redirect to
                res->contents = this->req->getRequestURI() + '/';
                return res;
            }
            
            // otherwise lets iterate through our defaultFiles and see if we can
            // find an index file that exits
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
    
    res->status = HTTP_200_OK;
    res->contents = uri;
    return res;
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
    abs_uri *u = getAbsoluteURI();
    std::string uri = u->contents;
    if (uri.empty()) {
        ERR_RESPONSE;
        return;
    }
    std::string method = this->req->getRequestMethod();
    
    // TODO: implement other HTTP methods
    if (method == "GET") {
        this->res = new Response(uri, this->sockfd);
        this->res->send(u->status);
    }
    else {
        ERR_RESPONSE;
        return;
    }
    
    printStatus();
}

Connection::~Connection() {
    delete req;
    delete res;
}
Connection::Connection() {}
Connection::Connection(Request *req, int sockfd, std::string location) {
    this->sockfd = sockfd;
    this->req = req;
    this->location = location;
}