//
//  Response.cpp
//  frostbite
//
//  Created by Conor Taylor on 14/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Response.h"

int Response::writeFile() {
    ifstream file;
    file.open(uri);
    if (!file) {
        return HTTP_404_NOT_FOUND;
    }
    else {
        char *buf = new char[1024];
        
        while (file) {
            file.read(buf, 1024);
            int n = write(this->sockfd, buf, 1024);
            if (n < 0) {
                cout << "Error: Unable to write to socket "
                    << this->sockfd << endl;
                return HTTP_500_INTERNAL_ERR;
            }
        }
        delete[] buf;
    }
    file.close();
    return HTTP_200_OK;
}

Response::Response() {}
Response::Response(std::string uri, int sockfd) {
    this->uri = uri;
    this->sockfd = sockfd;
    // complete the response and get the response code
    this->code = writeFile();
}

int Response::getCode() {
    return this->code;
}