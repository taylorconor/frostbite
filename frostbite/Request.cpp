//
//  Request.cpp
//  frostbite
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Request.h"

int Request::parse() {
    std::istringstream f(source);
    std::string line;
    std::string::size_type index;
    for (int i = 0; std::getline(f, line) && line != "\r"; i++) {
        if (i == 0) {
            // break the request type into its 3 parts
            std::vector<std::string> parts = Utils::explode(line, ' ');
            if (parts.size() == 3) {
                header["request-method"] = parts[0];
                header["request-uri"] = parts[1];
                header["request-http"] = parts[2];
            }
        }
        else {
            index = line.find(':', 0);
            if(index != std::string::npos) {
                header.insert(std::make_pair(
                    boost::algorithm::trim_copy(line.substr(0, index)),
                    boost::algorithm::trim_copy(line.substr(index + 1))
                ));
            }
        }
    }
    
    return 1;
}

Request::Request(std::string r) {
    this->source = r;
    this->parseStatus = parse();
}
Request::Request() {}

std::string Request::getRequestMethod() {
    return header["request-method"];
}

std::string Request::getRequestURI() {
    return header["request-uri"];
}

std::string Request::getRequestHTTP() {
    return header["request-http"];
}

std::string Request::getRequestParam(std::string param) {
    return header[param];
}

bool Request::isValid() {
    return (this->parseStatus >= 0);
}