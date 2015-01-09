//
//  Host.h
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__Host__
#define __frostbite__Host__

#include <stdio.h>
#include <string>
#include <vector>
#include <thread>

#include "Hostname.h"
#include "Request.h"
#include "Response.h"
#include "Connection.h"
#include "Utils.h"

class Host {
private:
    void watchPool();
    
    Hostname *hostname;
    std::string location;
    std::vector<Connection *> pool;
    std::thread watcher;
    bool shouldWatch;
public:
    Host();
    Host(Hostname *, std::string);
    
    void handleRequest(Request *, int);

    Hostname *getHostname();
};

#endif /* defined(__frostbite__Host__) */
