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
#include <thread>
#include <mutex>

#include "Request.h"
#include "Response.h"
#include "URI.h"

#define ERR_RESPONSE    this->res = new Response(_sockfd); \
                        this->res->send(HTTP_500_INTERNAL_ERR); \

class abs_uri {
public:
    int status;
    URI *contents;
    
    ~abs_uri() {
        delete this->contents;
    }
};

class Connection {
protected:
    abs_uri *absolute_uri();
    
    Request *req;
    Response *res;
    int _sockfd;
    bool completed;
    bool cache_override;
    std::string location;
    
public:
    virtual void print_status();
    std::thread *thread;
    static std::mutex *mtx;
    
    ~Connection();
    Connection();
    Connection(Request *, int, std::string);
    bool is_completed();
    int sockfd();
    std::string request_name();
    void set_cache_override(bool);
    
    virtual void handle_connection();
};

#endif /* defined(__frostbite__Connection__) */
