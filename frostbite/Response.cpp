//
//  Response.cpp
//  frostbite
//
//  Created by Conor Taylor on 14/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Response.h"

void Response::writeFile() {
    ifstream file;
    file.open(uri);
    if (!file) {
        cout << "Error: Can't open file " << uri << endl;
    }
    else {
        char *buf = new char[1024];
        
        while (file) {
            file.read(buf, 1024);
            int n = write(this->sockfd, buf, 1024);
            if (n < 0)
                Utils::error("ERROR writing to socket");
        }
        delete[] buf;
    }
    file.close();
}

Response::Response() {}
Response::Response(std::string uri, int sockfd) {
    this->uri = uri;
    this->sockfd = sockfd;
    writeFile();
}