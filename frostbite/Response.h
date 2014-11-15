//
//  Response.h
//  frostbite
//
//  Created by Conor Taylor on 14/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__Response__
#define __frostbite__Response__

#include <stdio.h>
#include <map>
#include <string>
#include <cstdio>
#include <cerrno>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Request.h"

class Response {
private:
    Request req;
    int sockfd;
    
    std::map<std::string,std::string> header;
    
    void processGetRequest();
public:
    Response(Request, int);
};

#endif /* defined(__frostbite__Response__) */
