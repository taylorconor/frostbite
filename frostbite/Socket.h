//
//  Socket.h
//  frostbite
//
//  Created by Conor Taylor on 29/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#ifndef __frostbite__Socket__
#define __frostbite__Socket__

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Utils.h"

class Socket {
public:
    static int openSocket(std::string host, int port);
};

#endif /* defined(__frostbite__Socket__) */
