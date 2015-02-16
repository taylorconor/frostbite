//
//  Request.cpp
//  frostbite
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Request.h"

int Request::parse() {
    std::istringstream f(_source);
    std::string line;
    std::string::size_type index;
    for (int i = 0; std::getline(f, line) && line != "\r"; i++) {
        if (i == 0) {
            // break the request type into its 3 parts
            std::vector<std::string> parts = Utils::explode(line, ' ');
            if (parts.size() == 3) {
                _header["request-method"] = parts[0];
                _header["request-uri"] = parts[1];
                
                // TODO: implement smart URL parsing
                /*size_t pos = parts[1].find(':');
                if (pos != std::string::npos) {
                    protocol = parts[1].substr(0,pos);
                    boost::algorithm::to_lower(protocol);
                    if (protocol.compare("https") == 0)
                        port = 443;
                    else
                        port = 80;
                }
                else {
                    // default to port 80 & http
                    port = 80;
                    protocol = "http";
                }*/
                _protocol = "http";
                _port = 80;
                
                _header["request-http"] = parts[2].substr(0,parts[2].length()-1);
            }
        }
        else {
            index = line.find(':', 0);
            if(index != std::string::npos) {
                std::string i=boost::algorithm::trim_copy(line.substr(0,index));
                std::string v=boost::algorithm::trim_copy(line.substr(index+1));
                _header[i] = v;
                
                // keep Host in it's own variable, since it's accessed so often
                if (i.compare("Host") == 0)
                    _host = v;
            }
        }
    }
    
    return 1;
}

Request::Request(std::string r) {
    this->_source = r;
    this->_status = parse();
    this->_pivot = this->_source.find("\r\n\r\n");
}
Request::Request() {}

std::string Request::method() {
    return _header["request-method"];
}

std::string Request::uri() {
    return _header["request-uri"];
}

std::string Request::http_version() {
    return _header["request-http"];
}

std::string Request::param(std::string param) {
    if (_header.find(param) != _header.end())
        return _header.at(param);
    else
        return "";
}

std::string Request::source() {
    return _source;
}

std::string Request::host() {
    return _host;
}

std::string Request::header() {
    return _source.substr(0,_pivot);
}

std::string Request::body() {
    return _source.substr(_pivot + 4);
}

int Request::port() {
    return _port;
}

void Request::set_param(std::string index, std::string val) {
    _header[index] = val;
}

bool Request::is_valid() {
    return (_status >= 0);
}