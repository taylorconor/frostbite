//
//  Host.cpp
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Host.h"

void Host::handleRequest(Request *req, int sockfd) {
    Connection *c = new Connection(req, sockfd, this->location);
    
    // add connection to the pool and notify the pool watcher thread
    this->pool.push_back(c);
    cv_watcher.notify_all();
}

void Host::watchPool() {
    // create a unique lock for the condition variable to wait on
    std::unique_lock<std::mutex> lck(mtx_watcher);
    
    while (this->shouldWatch) {
        // immediately wait on cv_watcher until data appears in the pool
        if (this->pool.size() == 0) {
            cv_watcher.wait(lck);
        }
        
        // make sure the pool has connections in it before operating
        if (this->pool.size() > 0) {
            for (int i = 0; i < this->pool.size(); i++) {
                // clean up any completed connections
                if (this->pool[i]->getStatus() == COMPLETED) {
                    // close the socket associated with the connection
                    close(this->pool[i]->getSockfd());
                    // delete the connection
                    delete this->pool[i];
                    // remove the deleted pointer from the pool
                    this->pool.erase(this->pool.begin() + i);
                }
                // handle any waiting connections
                else if (this->pool[i]->getStatus() == WAITING) {
                    this->pool[i]->handleConnection();
                }
            }
        }
    }
}

Host::Host() {}
Host::Host(Hostname *hostname, std::string location) {
    this->hostname = hostname;
    this->location = location;
    this->shouldWatch = true;
    this->watcher = std::thread(&Host::watchPool, this);
}

Hostname *Host::getHostname() {
    return this->hostname;
}