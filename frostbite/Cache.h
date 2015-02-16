//
//  Cache.h
//  frostbite
//
//  Created by Conor Taylor on 04/02/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#ifndef __frostbite__Cache__
#define __frostbite__Cache__

#include <stdio.h>
#include <string>
#include <map>
#include <fstream>
#include <unordered_map>
#include <regex>

#include "Utils.h"

struct CacheItem {
    std::string *addr;
    std::string hash;
    time_t created;
    time_t expiry;
};

class Cache {
private:
    static Cache *_instance;
    Cache();
    
    std::map<std::string, CacheItem> *cache_map;
    std::string _directory;
    std::string _console;

public:
    static Cache *instance();
    
    std::string *lookup(std::string);
    std::string hash(std::string);
    std::string directory();
    std::string console();
    void insert(std::string, std::string, std::string);
    
    void set_directory(std::string);
    void set_console(std::string);
};

#endif /* defined(__frostbite__Cache__) */
