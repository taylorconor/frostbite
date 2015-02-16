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
        return (*cache_map)[uri].addr;
    }
    return NULL;
}

std::string Cache::hash(std::string uri) {
    std::hash<std::string> hasher;
    return std::to_string(hasher(uri));
}

void Cache::insert(std::string uri, std::string hash) {
    (*cache_map)[uri].addr = new std::string(_directory + hash);
    if (_console.length() > 0) {
        std::ofstream f(_console+"cache");
        for (auto items : *cache_map) {
            f << items.first+"\n";
        }
        f.close();
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