//
//  Connection.h
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__Connection__
#define __frostbite__Connection__

#include <stdio.h>

#include "Request.h"
#include "Response.h"

class Connection {
private:
    std::string getAbsoluteURI();
    bool fileExists(std::string uri);
    void printStatus(bool);
    
    Request req;
    Response res;
    int sockfd;
    
public:
    Connection();
    Connection(Request, int);
    
    void handleConnection();
};

#endif /* defined(__frostbite__Connection__) */
