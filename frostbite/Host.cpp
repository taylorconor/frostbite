//
//  Host.cpp
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Host.h"

void Host::handleRequest(Request req, int sockfd) {
    Connection c = Connection(req, sockfd);
    
    // TODO: implement connection pooling
    //this->pool.push_back(c);
    c.handleConnection();
}

Host::Host() {}
Host::Host(Hostname hostname) {
    this->hostname = hostname;
}

Hostname Host::getHostname() {
    return this->hostname;
}