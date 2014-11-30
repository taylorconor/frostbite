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
#include "Utils.h"

#define HTTP_200_OK                 200
#define HTTP_301_MOVED              301
#define HTTP_400_BAD_REQUEST        400
#define HTTP_404_NOT_FOUND          404
#define HTTP_500_INTERNAL_ERR       500
#define HTTP_550_NO_PERMISSION      550

#define HTTP_200_OK_STR             "OK"
#define HTTP_301_MOVED_STR          "Moved Permanently"
#define HTTP_400_BAD_REQUEST_STR    "Bad Request"
#define HTTP_404_NOT_FOUND_STR      "Not Found"
#define HTTP_500_INTERNAL_ERR_STR   "Internal Server Error"
#define HTTP_550_NO_PERMISSION_STR  "No Permission"

class Response {
private:
    int writeFile();
    std::string getTitle(int);
    
    std::map<std::string,std::string> header;
    int sockfd;
    int code;
    std::string uri;

public:
    Response();
    Response(int);
    Response(std::string, int);
    int getCode();
    void send();
    void send(int);
};

#endif /* defined(__frostbite__Response__) */
