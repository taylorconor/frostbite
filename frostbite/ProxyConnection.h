//
//  ProxyConnection.h
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#ifndef __frostbite__ProxyConnection__
#define __frostbite__ProxyConnection__

#include <stdio.h>
#include <netdb.h>
#include <fcntl.h>

#include "Utils.h"
#include "Socket.h"
#include "Connection.h"
#include "Response.h"

class ProxyConnection : public Connection {
private:
    char *recbuf;
public:
    ProxyConnection(Request *, int);
    void handleConnection() override;
};

#endif /* defined(__frostbite__ProxyConnection__) */
