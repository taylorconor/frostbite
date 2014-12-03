//
//  Connection.h
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__Connection__
#define __frostbite__Connection__

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "Request.h"
#include "Response.h"
#include "URI.h"

typedef struct {
    int status;
    URI *contents;
} abs_uri;

class Connection {
private:
    abs_uri *getAbsoluteURI();
    void printStatus();
    
    Request *req;
    Response *res;
    int sockfd;
    std::string location;
    
public:
    ~Connection();
    Connection();
    Connection(Request *, int, std::string);
    
    void handleConnection();
};

#endif /* defined(__frostbite__Connection__) */
