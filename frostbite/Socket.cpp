//
//  Socket.cpp
//  frostbite
//
//  Created by Conor Taylor on 29/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#include "Socket.h"

int Socket::openSocket(std::string host, int port) {
    addrinfo host_info;
    addrinfo *host_info_list;
    int proxy_sockfd;
    ssize_t n;
    char portbuf[MAX_PORT_SIZE];
    
    // ensure it's initialised to zero
    memset(&host_info, 0, sizeof host_info);
    
    host_info.ai_family = AF_INET;
    host_info.ai_socktype = SOCK_STREAM;
    
    // convert port to c_str so it can be used for getaddrinfo
    sprintf(portbuf, "%i", port);
    n = getaddrinfo(host.c_str(), portbuf,
                    &host_info, &host_info_list);
    if (n == -1 || !host_info_list) {
        Utils::error("ERROR on getaddrinfo");
        return -1;
    }
    
    proxy_sockfd = socket(host_info_list->ai_family,
                          host_info_list->ai_socktype,
                          host_info_list->ai_protocol);
    
    if (proxy_sockfd == -1) {
        Utils::error("ERROR creating socket");
        return -1;
    }
    
    n = connect(proxy_sockfd, host_info_list->ai_addr,
                host_info_list->ai_addrlen);
    if (n == -1) {
        std::cout << "ERROR connecting to socket" << std::endl;
        return -1;
    }
    
    return proxy_sockfd;
}