//
//  Server.h
//  frostbite
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef SO_NOSIGPIPE
#define SO_NOSIGPIPE    MSG_NOSIGNAL
#endif

#ifndef __frostbite__Server__
#define __frostbite__Server__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "rapidjson/document.h"

#include "Host.h"
#include "ProxyHost.h"
#include "Hostname.h"
#include "Request.h"
#include "Response.h"
#include "Utils.h"

#define RECBUF              4096
#define DEFAULT_CONNECTIONS 256
#define MAX_ACCEPT_ATTEMPTS 10

#define MAX_PROXY_CONNS     64

#define PROXY_OFF           0
#define PROXY_OTHERS        1
#define PROXY_ALL           2

struct proxyStatus {
    int status;
    int connections;
    std::string cache;
    bool should_cache;
    std::string console;
    Host *host;
};

class Server {
private:
    static Server *_instance;
    Server();
    
    void init_server();
    void init_listen(int);
    void dispatch(Request *, int);
    int parse_config();
    
    int port;
    proxyStatus proxy;
    int _parse_status;
    std::string config;
    std::vector<Host *> hosts;
    
public:
    static Server *instance();
    
    void set_config(std::string);
    
    void listen();
    int parse_status();
};

#endif /* defined(__frostbite__Server__) */
