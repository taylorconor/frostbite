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

class abs_uri {
public:
    int status;
    URI *contents;
    
    ~abs_uri() {
        delete this->contents;
    }
};

class Connection {
private:
    abs_uri *getAbsoluteURI();
    void printStatus();
    
    Request *req;
    Response *res;
    int sockfd;
    bool completed;
    std::string location;
    
public:
    std::thread *thread;
    static std::mutex *mtx;
    
    ~Connection();
    Connection();
    Connection(Request *, int, std::string);
    bool isCompleted();
    int getSockfd();
    std::string getRequestName();
    
    void handleConnection();
};

#endif /* defined(__frostbite__Connection__) */
