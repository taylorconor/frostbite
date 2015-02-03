//
//  ProxyHost.cpp
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#include "ProxyHost.h"

void ProxyHost::handle_request(Request *req, int sockfd) {
    Connection *c = new ProxyConnection(req, sockfd);
    // add connection to the pool and notify the pool watcher thread
    pool.push(c);
    cv_watcher->notify_one();
}

ProxyHost::ProxyHost(int threads) {
    this->threads = threads;
    this->should_watch = true;
    this->cv_watcher = new std::condition_variable();
    for (int i = 0; i < this->threads; i++) {
        this->thread_pool.push_back(
            new std::thread(&ProxyHost::watch_pool, this));
    }
}