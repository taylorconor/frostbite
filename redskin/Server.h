//
//  Server.h
//  redskin
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __redskin__Server__
#define __redskin__Server__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Host.h"
#include "Hostname.h"
#include "Request.h"
#include "Response.h"
#include "Utils.h"

#define RECBUF  4096

class Server {
private:
    int port;
    std::vector<Host> hosts;
    
    void initServer();
    void initListen(int);
    void dispatch(Request, int);
public:
    
    Server(int port);
};

#endif /* defined(__redskin__Server__) */
