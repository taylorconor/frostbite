//
//  Cache.cpp
//  frostbite
//
//  Created by Conor Taylor on 04/02/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#include "Cache.h"

Cache *Cache::_instance = nullptr;

std::string *Cache::lookup(std::string uri) {
    if (cache_map->find(uri) != cache_map->end()) {
        // check if cache has expired. if it has, clean up and return null
        if ((*cache_map)[uri].expiry > 0 &&(*cache_map)[uri].expiry < time(0)) {
            remove((*cache_map)[uri].addr->c_str());
            delete (*cache_map)[uri].addr;
            cache_map->erase(uri);
            return NULL;
        }
        // valid cache hit
        return (*cache_map)[uri].addr;
    }
    return NULL;
}

std::string Cache::hash(std::string uri) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(uri));
}

void Cache::insert(std::string uri, std::string hash, std::string status) {
    bool should_cache = false;
    time_t expiry = 0;
    std::smatch match;
    if (status.length() == 0)
        should_cache = true;
    else {
        std::regex_search(status, match, std::regex("max-age=([0-9]+)"));
        if (!match.empty()) {
            std::string e = match.str(1);
            expiry = atoi(e.c_str());
            if (expiry > 0) {
                should_cache = true;
                expiry += time(0);
            }
        }
    }
    
    if (should_cache) {
        (*cache_map)[uri].addr = new std::string(_directory + hash);
        (*cache_map)[uri].created = time(0);
        (*cache_map)[uri].expiry = expiry;
        (*cache_map)[uri].hash = hash;
        
        if (_console.length() > 0) {
            std::ofstream f(_console+"cache");
            for (auto items : *cache_map) {
                f <<    "<code>" << Utils::time_string(items.second.created) <<
                "</code>&nbsp;<a href=\"/admin/cache/" << items.second.hash
                << "\" target=\"_blank\">" << items.first.substr(0,100)
                << "</a><br>";
            }
            f.close();
        }
    }
}

std::string Cache::directory() {
    return _directory;
}

std::string Cache::console() {
    return _console;
}

void Cache::set_directory(std::string directory) {
    this->_directory = directory;
    // now delete all previously cached items from this directory
    system(("exec rm -r "+directory+"*").c_str());
}

void Cache::set_console(std::string console) {
    this->_console = console;
}

Cache::Cache() {
    cache_map = new std::map<std::string, CacheItem>();
}
Cache *Cache::instance() {
    if (_instance == NULL) {
        _instance = new Cache();
    }
    return _instance;
}