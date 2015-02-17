//
//  Response.cpp
//  frostbite
//
//  Created by Conor Taylor on 14/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Response.h"

long status;

int Response::write_file() {
    // uri doesn't *have* to be defined, but if it isn't there's a big problem
    if (!uri || this->uri->is_empty())
        return HTTP_500_INTERNAL_ERR;
        
    std::ifstream file;
    file.open(uri->src());
    std::string mime_type = uri->mime();
    if (mime_type.length() > 0)
        header["Content-Type"] = mime_type;
    if (!file) {
        return HTTP_404_NOT_FOUND;
    }
    else if (uri->clean_ext() == ".php") {
        const char *h = ("HTTP/1.1 200 OK\n" +
                         Utils::dump_map(header)).c_str();
        if (write(this->sockfd, h, strlen(h)) < 0) {
            Utils::error("ERROR unable to write to socket " +
                         std::to_string(sockfd));
            return HTTP_500_INTERNAL_ERR;
        }
        
        std::string cmd;
        if (req->method().compare("POST") == 0) {
            cmd =   "echo \""+req->body()+"\" | "
                    "REDIRECT_STATUS=CGI "
                    "REQUEST_METHOD=POST "
                    "SCRIPT_FILENAME="+uri->src()+" "
                    "CONTENT_TYPE=application/x-www-form-urlencoded "
                    "CONTENT_LENGTH="+std::to_string(req->body().length())+" "
                    "php-cgi";
        }
        else {
            // make sure to execute the php script from its own parent directory
            // for filesystem reference reasons
            cmd +=  "cd "+uri->parent_dir()+" && php-cgi "+uri->src();
        }

        std::cout << cmd << std::endl;
        char *buf = new char[MAX_BUF];
        FILE *fp = popen(cmd.c_str(), "r");
        if (!fp) {
            delete[] buf;
            return HTTP_500_INTERNAL_ERR;
        }
        
        while (fgets(buf, MAX_BUF-1, fp) != NULL) {
            long status = write(this->sockfd, buf, strlen(buf));
            if (status < 0) {
                Utils::error("ERROR unable to write to socket " +
                             std::to_string(sockfd));
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
        std::string exclude = "http://localhost:1234/admin/cache";
        if (req->uri().substr(0,exclude.length()).compare(exclude) != 0) {
            const char *h = ("HTTP/1.1 200 OK\n" +
                             Utils::dump_map(this->header) + "\n").c_str();
            if (write(this->sockfd, h, strlen(h)) < 0) {
                Utils::error("ERROR unable to write to socket " +
                             std::to_string(sockfd));
                return HTTP_500_INTERNAL_ERR;
            }
        }
        
        char *buf = new char[MAX_BUF];
        
        while (file) {
            file.read(buf, MAX_BUF);
            long status = write(this->sockfd, buf, file.gcount());
            if (status < 0) {
                Utils::error("ERROR unable to write to socket " +
                             std::to_string(sockfd));
                delete[] buf;
                return HTTP_500_INTERNAL_ERR;
            }
        }
        delete[] buf;
    }
    file.close();
    return HTTP_200_OK;
}

std::string Response::title(int code) {
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
    _code = write_file();
    
    if (_code != HTTP_200_OK) {
        std::string h = "HTTP/1.1 " + std::to_string(_code) + " " +
            title(_code) + "\n" + Utils::dump_map(header);
        write(sockfd, h.c_str(), strlen(h.c_str()));
    }
}

void Response::send(int code) {
    // force response for the user-specified code
    this->_code = code;
    
    if (_code == HTTP_200_OK) {
        // try to send using the normal send method
        send();
        return;
    }
    
    // a 301 Permenantly Moved status requires a location in its response so
    // the browser knows which page to resend its request to
    if (_code == HTTP_301_MOVED && uri)
        header["Location"] = this->uri->src();
    
    std::string h = "HTTP/1.1 " + std::to_string(code) + " " +
        title(code) + "\n" + Utils::dump_map(header);
    write(sockfd, h.c_str(), strlen(h.c_str()));
}

Response::Response() {}
Response::Response(Request *req, int sockfd) {
    this->sockfd = sockfd;
    this->req = req;
    
    header["Server"] = "frostbite";
    header["Accept-Ranges"] = "bytes";
    header["Connection"] = "Keep-Alive";
}
Response::Response(URI *uri, Request *req, int sockfd) :
Response::Response(req, sockfd) {
    this->uri = uri;
}

int Response::code() {
    return _code;
}