//
//  Response.h
//  frostbite
//
//  Created by Conor Taylor on 14/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__Response__
#define __frostbite__Response__

#include <stdio.h>
#include <map>
#include <string>
#include <cstdio>
#include <cerrno>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Request.h"

#define HTTP_200_OK             200
#define HTTP_400_BAD_REQUEST    400
#define HTTP_404_NOT_FOUND      404
#define HTTP_500_INTERNAL_ERR   500
#define HTTP_550_NO_PERMISSION  550

class Response {
private:
    int writeFile();
    
    Request req;
    std::map<std::string,std::string> header;
    int sockfd;
    int code;
    std::string uri;

public:
    Response();
    Response(std::string, int);
    int getCode();
};

#endif /* defined(__frostbite__Response__) */
