//
//  Response.cpp
//  frostbite
//
//  Created by Conor Taylor on 14/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Response.h"

long status;

int Response::writeFile() {
    // uri doesn't *have* to be defined, but if it isn't there's a big problem
    if (!uri || this->uri->isEmpty())
        return HTTP_500_INTERNAL_ERR;
        
    ifstream file;
    file.open(uri->src());
    if (!file) {
        return HTTP_404_NOT_FOUND;
    }
    else if (uri->cleanExt() == ".php") {
        const char *r = "HTTP/1.1 200 OK\n";
        if (write(this->sockfd, r, strlen(r)) < 0) {
            cout << "Error: Unable to write to socket " << this->sockfd << endl;
            return HTTP_500_INTERNAL_ERR;
        }
        const char *h = (Utils::dump_map(this->header) + "\n").c_str();
        if (write(this->sockfd, h, strlen(h)) < 0) {
            cout << "Error: Unable to write to socket " << this->sockfd << endl;
            return HTTP_500_INTERNAL_ERR;
        }
        
        // make sure to execute the php script from its own parent directory,
        // for filesystem reference reasons
        std::string cmd = "cd "+uri->parentDir()+" && php "+uri->src();
        
        char *buf = new char[1024];
        FILE *fp = popen(cmd.c_str(), "r");
        if (!fp) {
            delete[] buf;
            return HTTP_500_INTERNAL_ERR;
        }
        
        while (fgets(buf, sizeof(buf)-1, fp) != NULL) {
            long status = write(this->sockfd, buf, strlen(buf));
            if (status < 0) {
                cout << "Error: Unable to write to socket "
                    << this->sockfd << "; " << strerror(errno) << endl;
                pclose(fp);
                delete[] buf;
                return HTTP_500_INTERNAL_ERR;
            }
        }
        pclose(fp);
        delete[] buf;
    }
    else {
        // expecting a 200 success unless the socket fails while writing
        const char *r = "HTTP/1.1 200 OK\n";
        if (write(this->sockfd, r, strlen(r)) < 0) {
            cout << "Error: Unable to write to socket " << this->sockfd << endl;
            return HTTP_500_INTERNAL_ERR;
        }
        const char *h = (Utils::dump_map(this->header) + "\n").c_str();
        if (write(this->sockfd, h, strlen(h)) < 0) {
            cout << "Error: Unable to write to socket " << this->sockfd << endl;
            return HTTP_500_INTERNAL_ERR;
        }
        
        char *buf = new char[1024];
        
        while (file) {
            file.read(buf, 1024);
            long status = write(this->sockfd, buf, file.gcount());
            if (status < 0) {
                cout << "Error: Unable to write to socket "
                    << this->sockfd << "; " << strerror(errno) << endl;
                delete[] buf;
                return HTTP_500_INTERNAL_ERR;
            }
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
    if (this->code == HTTP_301_MOVED && uri)
        header["Location"] = this->uri->src();
    
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
Response::Response(URI *uri, int sockfd) {
    this->uri = uri;
    this->sockfd = sockfd;
    
    header["Server"] = "frostbite";
    header["Accept-Ranges"] = "bytes";
    header["Connection"] = "Keep-Alive";
}

int Response::getCode() {
    return this->code;
}