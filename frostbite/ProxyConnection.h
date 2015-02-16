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
#include "Cache.h"

#define PSTATUS_INTERNAL_ERROR          -1
#define PSTATUS_OK                      0
#define PSTATUS_METHOD_NOT_SUPPORTED    1
#define PSTATUS_SOCK_INIT_FAIL          2
#define PSTATUS_SEND_FAIL               3

#define OUTPUT_STR_LEN                  60

class ProxyConnection : public Connection {
private:
    char *recbuf;
    int status;
    Cache *cache;
    bool should_cache;
public:
    void print_status() override;
    ProxyConnection(Request *, int);
    ProxyConnection(Request *, int, Cache *);
    void handle_connection() override;
    void set_cache_override(bool) override;
};

#endif /* defined(__frostbite__ProxyConnection__) */
