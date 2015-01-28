//
//  ProxyHost.cpp
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#include "ProxyHost.h"

void ProxyHost::handleRequest(Request *req, int sockfd) {
    Connection *c = new ProxyConnection(req, sockfd);
    // add connection to the pool and notify the pool watcher thread
    pool.push(c);
    cv_watcher->notify_one();
}

void ProxyHost::watchPool() {
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

ProxyHost::ProxyHost(int threads) {
    this->threads = threads;
    this->shouldWatch = true;
    this->cv_watcher = new std::condition_variable();
    for (int i = 0; i < this->threads; i++) {
        this->thread_pool.push_back(
            new std::thread(&ProxyHost::watchPool, this));
    }
}