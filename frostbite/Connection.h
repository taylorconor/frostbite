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
    Request req;
    Response res;
    int sockfd;
    
public:
    Connection(int);
};

#endif /* defined(__frostbite__Connection__) */
