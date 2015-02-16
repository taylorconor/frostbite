//
//  ProxyHost.h
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#ifndef __frostbite__ProxyHost__
#define __frostbite__ProxyHost__

#include <stdio.h>
#include <vector>

#include "Host.h"
#include "ProxyConnection.h"
#include "Cache.h"

class ProxyHost : public Host {
private:
    bool should_cache;
    Cache *cache;
    std::vector<std::string> blocklist;
    
    void watch_pool();
    void watch_blocklist();
public:
    void handle_request(Request *, int) override;
    
    ProxyHost(int);
    ProxyHost(int, std::string, std::string);
};

#endif /* defined(__frostbite__ProxyHost__) */
