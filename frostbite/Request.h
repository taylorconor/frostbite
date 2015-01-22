//
//  Request.h
//  frostbite
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__Request__
#define __frostbite__Request__

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>

#include <boost/algorithm/string.hpp>

#include "Utils.h"

using namespace std;

class Request {
private:
    int parse();
    
    map<std::string,std::string> header;
    std::string source;
    int parseStatus;
    
public:
    Request();
    Request(std::string);
    
    std::string getRequestMethod();
    std::string getRequestURI();
    std::string getRequestHTTP();
    std::string getRequestParam(std::string);
    bool isValid();
};

#endif /* defined(__frostbite__Request__) */
