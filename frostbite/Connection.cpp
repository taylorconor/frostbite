//
//  Connection.cpp
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//
//  Represents a single connection to the server
//

#include "Connection.h"

std::mutex *Connection::mtx = new std::mutex();

// TODO: abstract this to the config file
std::vector<std::string> defaultFiles =
    {"index.html", "index.htm", "index.php", "index.php3", "index.php4",
        "index.php5", "index.phtml"};

abs_uri *Connection::absolute_uri() {
    abs_uri *ret = new abs_uri;
    
    std::string r = req->uri();
    std::string h = req->host();
    
    // make sure that r doesn't begin with it's hostname, which can happen if
    // the server is proxying but accessing a frostbite-served file
    size_t pos;
    if ((pos = r.find(h)) != std::string::npos)
        r = r.substr(pos + h.length());
    if (r.front() == '/')
        r.erase(0,1);

    URI *uri = new URI(location + r);
    
    if (uri->is_valid()) {
        if(uri->is_directory()) {
            // if there's no trailing slash then the browser is going to get
            // really confused. we need to responde with a 301 Moved Permanently
            // to tell the browser that the real location has a trailing slash
            if (uri->src().back() != '/') {
                ret->status = HTTP_301_MOVED;
                // here we'll add the slash so that Response will know what to
                // tell the browser to redirect to
                ret->contents = new URI(req->uri() + '/');
                
                delete uri;
                return ret;
            }
            
            // otherwise lets iterate through our defaultFiles and see if we can
            // find an index file that exits
            for (int i = 0; i < defaultFiles.size(); i++) {
                if (Utils::exists(uri->src()+defaultFiles[i])) {
                    uri->append(defaultFiles[i]);
                    break;
                }
            }
        }
        else if(uri->is_file()) {
            // it's a file, uri is already the absolute uri
        }
    }
    else {
        ret->status = HTTP_404_NOT_FOUND;
        ret->contents = nullptr;
        delete uri;
        return ret;
    }
    
    ret->status = HTTP_200_OK;
    ret->contents = uri;
    return ret;
}

void Connection::print_status() {
    int code = res->code();
    
    std::string output;
    
    if (code > 400)
        output += "ERR\t";
    else
        output += "OK\t";
    
    output += std::to_string(code)+"\t"+req->method()+"\t"+req->uri()+"\n";
    
    mtx->lock();
    std::cout << output;
    mtx->unlock();
}

void Connection::handle_connection() {
    if (!this->req->is_valid()) {
        ERR_RESPONSE;
        return;
    }
    abs_uri *u = absolute_uri();
    URI *uri = u->contents;
    std::string method = req->method();
    
    // TODO: implement other HTTP methods
    if (method.compare("GET") == 0) {
        res = new Response(uri, _sockfd);
        res->send(u->status);
    }
    else {
        res = new Response(_sockfd);
        res->send(HTTP_405_METHOD_NOT_ALLOWED);
        delete uri;
        return;
    }
    
    delete u;
    print_status();
    completed = true;
}

Connection::~Connection() {
    delete req;
    if (res != nullptr)
        delete res;
}
Connection::Connection() {}
Connection::Connection(Request *req, int sockfd, std::string location) {
    this->_sockfd = sockfd;
    this->req = req;
    this->location = location;
    this->completed = false;
}

int Connection::sockfd() {
    return _sockfd;
}
bool Connection::is_completed() {
    return completed;
}

std::string Connection::request_name() {
    return req->uri();
}