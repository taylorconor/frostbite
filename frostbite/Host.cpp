//
//  Host.cpp
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Host.h"

void Host::handleRequest(Request *req, int sockfd) {
    ConnectionWrapper *c = new ConnectionWrapper;
    c->connection = new Connection(req, sockfd, location);
    
    // add connection to the pool and notify the pool watcher thread
    pool.push_back(c);
    cv_watcher->notify_all();
}

void Host::watchPool() {
    // create a unique lock for the condition variable to wait on
    std::unique_lock<std::mutex> lck(mtx_watcher);
    
    while (shouldWatch) {
        // immediately wait on cv_watcher until data appears in the pool
        if (pool.size() == 0) {
            cv_watcher->wait(lck);
        }
        
        // make sure the pool has connections in it before operating
        if (pool.size() > 0) {
            for (int i = 0; i < pool.size(); i++) {
                // clean up any completed connections
                if (!pool[i]->isWaiting && pool[i]->connection->isCompleted()) {
                    // close the socket associated with the connection
                    close(pool[i]->connection->getSockfd());
                    // delete the connection's thread
                    pool[i]->connection->thread->join();
                    delete pool[i]->connection->thread;
                    // delete the connection
                    delete pool[i];
                    // remove the deleted pointer from the pool
                    pool.erase(pool.begin() + i);
                }
                // handle any waiting connections
                else if (this->pool[i]->isWaiting) {
                    pool[i]->isWaiting = false;
                    pool[i]->connection->thread =
                        new std::thread(&Connection::handleConnection,
                                    this->pool[i]->connection);
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
    this->cv_watcher = new std::condition_variable();
    this->watcher = std::thread(&Host::watchPool, this);
}

Hostname *Host::getHostname() {
    return this->hostname;
}