//
//  Host.cpp
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Host.h"

void Host::handleRequest(Request *req, int sockfd) {
    Connection *c = new Connection(req, sockfd);
    
    // TODO: implement connection pooling & callbacks, this code is shit
    //this->pool.push_back(c);
    c->handleConnection();
    
    delete c;
}

Host::Host() {}
Host::Host(Hostname *hostname, std::string location) {
    this->hostname = hostname;
    this->location = location;
}

Hostname *Host::getHostname() {
    return this->hostname;
}