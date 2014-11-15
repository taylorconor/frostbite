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

#include "Utils.h"

using namespace std;

class Request {
private:
    map<string,string> header;
    string source;
    int parseStatus;
    
    int parse();
public:
    Request(string);
    void printStatus();
    
    string getRequestMethod();
    string getRequestURI();
    string getRequestHTTP();
    bool getValidity();
};

#endif /* defined(__frostbite__Request__) */
