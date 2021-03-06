//
//  Host.h
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__Host__
#define __frostbite__Host__

#include <stdio.h>
#include <string>
#include <vector>
#include <thread>
#include <queue>
#include <condition_variable>

#include "Hostname.h"
#include "Request.h"
#include "Response.h"
#include "Connection.h"
#include "Utils.h"

class Host {
protected:
    virtual void watch_pool();
    
    Hostname *_hostname;
    std::string location;
    std::queue<Connection *> pool;
    std::vector<std::thread *> thread_pool;
    std::thread watcher;
    std::condition_variable *cv_watcher;
    std::mutex mtx_watcher;
    std::mutex mtx_pool;
    int threads;
    bool should_watch;
public:
    Host();
    Host(Hostname *, std::string, int);
    
    virtual void handle_request(Request *, int);

    Hostname *hostname();
};

#endif /* defined(__frostbite__Host__) */
