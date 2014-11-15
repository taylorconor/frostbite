//
//  Host.cpp
//  redskin
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Host.h"

void Host::handleRequest(Request req, int sockfd) {
    req.printStatus();
    // only create response if valid request
    if (req.getValidity())
        Response res = Response(req, sockfd);
}

Host::Host(Hostname hostname) : hostname(hostname) {

}

Hostname Host::getHostname() {
    return this->hostname;
}