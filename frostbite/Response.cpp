//
//  Response.cpp
//  frostbite
//
//  Created by Conor Taylor on 14/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Response.h"

void Response::processGetRequest() {
    string uri = "/Users/Conor/Documents/Projects/frostbite/srv"
        +this->req.getRequestURI();
    if (uri.back() == '/')
        uri += "index.html";
    
    ifstream file;
    file.open(uri.c_str());
    if (!file) {
        cout << "Error: Can't open file " << uri << endl;
    }
    else {
        string dat, accum;
        while (file) {
            file >> accum;
            dat += ' '+accum;
        }
        int n = write(this->sockfd, dat.c_str(), strlen(dat.c_str()));
        if (n < 0)
            Utils::error("ERROR writing to socket");
    }
}

Response::Response(Request req, int sockfd) : req(req) {
    this->sockfd = sockfd;
    
    string method = this->req.getRequestMethod();
    if (method == "GET") {
        processGetRequest();
    }
}