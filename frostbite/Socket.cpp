//
//  Socket.cpp
//  frostbite
//
//  Created by Conor Taylor on 29/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#include "Socket.h"

int Socket::openSocket(std::string host, int port) {
    // begin by opening socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        Utils::error("ERROR opening socket");
    
    struct sockaddr_in serv_addr;
    
    // retrieve hostname of host
    struct hostent *server = gethostbyname(host.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(port);
    
    if (connect(sockfd,(struct sockaddr *)&serv_addr,
                sizeof(serv_addr))< 0)
        Utils::error("ERROR connecting");
    
    return sockfd;
}