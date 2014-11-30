//
//  Response.cpp
//  frostbite
//
//  Created by Conor Taylor on 14/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Response.h"

#define safeWrite(x,y,z) if (write(x,y,z) < 0) { \
                            cout << "Error: Unable to write to socket " \
                                << this->sockfd << endl; \
                            return HTTP_500_INTERNAL_ERR; }

int Response::writeFile() {
    // uri doesn't *have* to be defined, but if it isn't there's a big problem
    if (this->uri.empty())
        return HTTP_500_INTERNAL_ERR;
        
    ifstream file;
    file.open(uri);
    if (!file) {
        return HTTP_404_NOT_FOUND;
    }
    else {
        // expecting a 200 success unless the socket fails while writing
        const char *r = "HTTP/1.1 200 OK\n";
        safeWrite(this->sockfd, r, strlen(r));
        const char *h = (Utils::dump_map(this->header) + "\n").c_str();
        safeWrite(this->sockfd, h, strlen(h));
        
        char *buf = new char[1024];
        
        while (file) {
            file.read(buf, 1024);
            safeWrite(this->sockfd, buf, file.gcount());
        }
        delete[] buf;
    }
    file.close();
    return HTTP_200_OK;
}

std::string Response::getTitle(int code) {
    switch (code) {
        case HTTP_200_OK:
            return HTTP_200_OK_STR;
        case HTTP_301_MOVED:
            return HTTP_301_MOVED_STR;
        case HTTP_400_BAD_REQUEST:
            return HTTP_400_BAD_REQUEST_STR;
        case HTTP_404_NOT_FOUND:
            return HTTP_404_NOT_FOUND_STR;
        case HTTP_500_INTERNAL_ERR:
            return HTTP_500_INTERNAL_ERR_STR;
        case HTTP_550_NO_PERMISSION:
        default:
            return HTTP_550_NO_PERMISSION_STR;
    }
}

void Response::send() {
    // complete the response and get the response code
    this->code = writeFile();
    
    if (this->code != HTTP_200_OK) {
        std::string s = "HTTP/1.1 " + to_string(this->code) + " " +
        getTitle(this->code) + "\n";
        write(this->sockfd, s.c_str(), strlen(s.c_str()));
        std::string h = Utils::dump_map(this->header);
        write(this->sockfd, h.c_str(), strlen(h.c_str()));
    }
}

void Response::send(int code) {
    // force response for the user-specified code
    this->code = code;
    
    if (this->code == HTTP_200_OK) {
        // try to send using the normal send method
        send();
        return;
    }
    
    // a 301 Permenantly Moved status requires a location in its response so
    // the browser knows which page to resend its request to
    if (this->code == HTTP_301_MOVED)
        header["Location"] = this->uri;
    
    std::string s = "HTTP/1.1 " + to_string(this->code) + " " +
    getTitle(this->code) + "\n";
    write(this->sockfd, s.c_str(), strlen(s.c_str()));
    std::string h = Utils::dump_map(this->header);
    write(this->sockfd, h.c_str(), strlen(h.c_str()));
}

Response::Response() {}
Response::Response(int sockfd) {
    this->sockfd = sockfd;
    
    header["Server"] = "frostbite";
    header["Accept-Ranges"] = "bytes";
    header["Connection"] = "Keep-Alive";
}
Response::Response(std::string uri, int sockfd) {
    this->uri = uri;
    this->sockfd = sockfd;
    
    header["Server"] = "frostbite";
    header["Accept-Ranges"] = "bytes";
    header["Connection"] = "Keep-Alive";
}

int Response::getCode() {
    return this->code;
}