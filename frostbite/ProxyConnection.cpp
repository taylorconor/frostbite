//
//  ProxyConnection.cpp
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#include "ProxyConnection.h"

size_t ProxyConnection::writer(char *s, size_t size, size_t nmemb, void *f) {
    ProxyConnection *ptr = (ProxyConnection *)f;
    
    long status = write(ptr->sockfd, s, nmemb);
    if (status < 0) {
        cout << "Error: Unable to write to socket "
            << ptr->sockfd << "; " << strerror(errno) << endl;
    }
    
    return size * nmemb;
}

void ProxyConnection::handleConnection() {
    if (!this->req->isValid()) {
        ERR_RESPONSE;
        return;
    }
    
    cout << "Handling proxy request for: " << req->getRequestURI() << " with method " << req->getRequestMethod() << endl;
    
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers=NULL;
    std::ifstream  fin(req->getSource());
    std::string    file_line;
    while(std::getline(fin, file_line)) {
        curl_slist_append(headers, file_line.c_str());
    }
    
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, req->getRequestURI().c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEHEADER, writer);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        
        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        
        curl_easy_cleanup(curl);
    }
    
    this->completed = true;
}

ProxyConnection::ProxyConnection(Request *req, int sockfd) {
    this->req = req;
    this->res = nullptr;
    this->sockfd = sockfd;
    this->completed = false;
}