//
//  Request.cpp
//  frostbite
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Request.h"

int Request::parse() {
    std::istringstream f(this->source);
    std::string line;
    for (int i = 0; getline(f, line); i++) {
        // parse the request type, e.g. "GET / HTTP/1.1"
        if (i == 0) {
            // break the request type into its 3 parts
            std::vector<std::string> parts = Utils::explode(line, ' ');
            if (parts.size() == 3) {
                header["request-method"] = parts[0];
                header["request-uri"] = parts[1];
                header["request-http"] = parts[2];
            }
            else {
                // invalid header - expected e.g. "GET / HTTP/1.1"
                return -1;
            }
        }
        else {
            std::vector<std::string> parts = Utils::explode(line, ':', 2);  
            if (parts.size() == 2)
                header[parts[0]] = parts[1];
        }
    }
    return 1;
}

Request::Request(std::string r) {
    this->source = r;
    this->parseStatus = parse();
}
Request::Request() {}

string Request::getRequestMethod() {
    return header["request-method"];
}

string Request::getRequestURI() {
    return header["request-uri"];
}

string Request::getRequestHTTP() {
    return header["request-http"];
}

bool Request::isValid() {
    return (this->parseStatus >= 0);
}