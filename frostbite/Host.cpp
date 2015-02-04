//
//  Host.cpp
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Host.h"

void Host::handle_request(Request *req, int sockfd) {
    Connection *c = new Connection(req, sockfd, location);
    // add connection to the pool and notify the pool watcher thread
    pool.push(c);
    cv_watcher->notify_one();
}

void Host::watch_pool() {
    while (should_watch) {
        // create a unique lock for the condition variable to wait on
        std::unique_lock<std::mutex> lck(mtx_watcher);
        
        // immediately wait on cv_watcher until pool requests this thread
        cv_watcher->wait(lck, [this](){return !pool.empty();});
        
        // take an item from the pool
        Connection *item = pool.front();
        pool.pop();
        lck.release()->unlock();
        
        item->handle_connection();
        item->print_status();
        close(item->sockfd());
        delete item;
    }
}

Host::Host() {}
Host::Host(Hostname *hostname, std::string location, int threads) {
    this->_hostname = hostname;
    this->location = location;
    this->should_watch = true;
    this->cv_watcher = new std::condition_variable();
    this->threads = threads;
    for (int i = 0; i < this->threads; i++) {
        this->thread_pool.push_back(new std::thread(&Host::watch_pool, this));
    }
}

Hostname *Host::hostname() {
    return this->_hostname;
}