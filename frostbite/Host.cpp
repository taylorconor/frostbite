//
//  Host.cpp
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Host.h"

void Host::handleRequest(Request *req, int sockfd) {
    Connection *c = new Connection(req, sockfd, location);
    // add connection to the pool and notify the pool watcher thread
    pool.push(c);
    cv_watcher->notify_one();
}

void Host::watchPool() {
    while (shouldWatch) {
        // create a unique lock for the condition variable to wait on
        std::unique_lock<std::mutex> lck(mtx_watcher);
        
        // immediately wait on cv_watcher until pool requests this thread
        cv_watcher->wait(lck);
        
        // take an item from the pool
        mtx_pool.lock();
        Connection *item = pool.front();
        pool.pop();
        mtx_pool.unlock();
        
        item->handleConnection();
        close(item->getSockfd());
        delete item;
    }
}

Host::Host() {}
Host::Host(Hostname *hostname, std::string location, int threads) {
    this->hostname = hostname;
    this->location = location;
    this->shouldWatch = true;
    this->cv_watcher = new std::condition_variable();
    this->threads = threads;
    for (int i = 0; i < this->threads; i++) {
        this->thread_pool.push_back(new std::thread(&Host::watchPool, this));
    }
}

Hostname *Host::getHostname() {
    return this->hostname;
}