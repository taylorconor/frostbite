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
#include <string.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Request.h"
#include "Response.h"
#include "Utils.h"

#define RECBUF  4096

class Server {
private:
    int port;
    
    void initServer();
    void initListen(int);
public:
    
    Server(int port);
};

#endif /* defined(__redskin__Server__) */
