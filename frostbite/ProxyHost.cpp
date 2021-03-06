//
//  ProxyHost.cpp
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#include "ProxyHost.h"

void ProxyHost::handle_request(Request *req, int sockfd) {
    Connection *c;
    if (should_cache)
        c = new ProxyConnection(req, sockfd, cache);
    else
        c = new ProxyConnection(req, sockfd);
    
    // add connection to the pool and notify the pool watcher thread
    pool.push(c);
    cv_watcher->notify_one();
}

void ProxyHost::watch_pool() {
    while (should_watch) {
        // create a unique lock for the condition variable to wait on
        std::unique_lock<std::mutex> lck(mtx_watcher);
        
        // immediately wait on cv_watcher until pool requests this thread
        cv_watcher->wait(lck, [this](){return !pool.empty();});
        
        // take an item from the pool
        Connection *item = pool.front();
        pool.pop();
        lck.unlock();
        
        std::string *src;
        if (std::find(blocklist.begin(), blocklist.end(), item->request_host())
            != blocklist.end()) {
            std::cout << "BLOCKED: " << item->request_host() << std::endl;
        }
        else if ((src = cache->lookup(item->request_name())) != NULL) {
            std::ifstream file;
            file.open(*src);
            
            char *buf = new char[MAX_BUF];
            
            while (file) {
                file.read(buf, MAX_BUF);
                long status = write(item->sockfd(), buf, file.gcount());
                if (status < 0) {
                    Utils::error("ERROR unable to write to socket " +
                                 std::to_string(item->sockfd()));
                    file.close();
                    close(item->sockfd());
                    delete item;
                    continue;
                }
            }
            item->set_cache_override(true);
            
            file.close();
            delete[] buf;
        }
        else {
            item->handle_connection();
        }
        item->print_status();
        close(item->sockfd());
        delete item;
    }
}

void ProxyHost::watch_blocklist() {
    while (should_watch) {
        std::ifstream file(cache->console()+"blocklist");
        unsigned long n = std::count(std::istreambuf_iterator<char>(file),
                                     std::istreambuf_iterator<char>(), '\n');
        
        file.clear();
        file.seekg(0, std::ios::beg);
        
        if (n > blocklist.size()) {
            std::string s;
            unsigned long prev = blocklist.size();
            
            for (unsigned long i = 0; i < n; i++) {
                std::getline(file, s);
                
                // skip n lines
                if (i >= prev)
                    blocklist.push_back(s);
            }
        }
        
        file.close();
        
        sleep(1);
    }
}

ProxyHost::ProxyHost(int threads) {
    this->threads = threads;
    this->should_watch = true;
    this->cv_watcher = new std::condition_variable();
    this->cache = nullptr;
    this->should_cache = false;
    for (int i = 0; i < this->threads; i++) {
        this->thread_pool.push_back(
            new std::thread(&ProxyHost::watch_pool, this));
    }
}

ProxyHost::ProxyHost(int t, std::string cache, std::string console) :
ProxyHost::ProxyHost(t) {
    this->cache = Cache::instance();
    this->cache->set_directory(cache);
    this->cache->set_console(console);
    // now delete all previously cached items from the console directory
    system(("exec rm -r "+console+"*").c_str());
    this->should_cache = true;
    this->thread_pool.push_back(new std::thread(&ProxyHost::watch_blocklist,
                                                this));
}