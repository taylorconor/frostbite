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

class Request {
private:
    int parse();
    
    std::map<std::string,std::string> _header;
    std::string _source;
    std::string _protocol;
    std::string _host;
    std::size_t _pivot;
    int _status;
    int _port;
    
public:
    Request();
    Request(std::string);
    
    std::string method();
    std::string uri();
    std::string http_version();
    std::string param(std::string);
    std::string source();
    std::string protocol();
    std::string host();
    std::string header();
    std::string body();
    int port();
    void set_param(std::string, std::string);
    bool is_valid();
};

#endif /* defined(__frostbite__Request__) */
