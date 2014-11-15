//
//  Request.h
//  redskin
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __redskin__Request__
#define __redskin__Request__

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
    
    int parse();
public:
    Request(string);
    
    string getRequestMethod();
    string getRequestURI();
    string getRequestHTTP();
};

#endif /* defined(__redskin__Request__) */
