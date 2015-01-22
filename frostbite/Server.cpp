//
//  Server.cpp
//  frostbite
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Server.h"

Server *Server::instance = nullptr;

// dispatch the request to the correct host for handling
void Server::dispatch(Request *req, int newsockfd) {
    std::string reqHost = req->getRequestParam("Host");
    bool handled = false;
    for (int i = 0; i < hosts.size(); i++) {
        Hostname *h = hosts[i]->getHostname();
        if (h->contains(reqHost)) {
            hosts[i]->handleRequest(req, newsockfd);
            handled = true;
            break;
        }
    }
    // delete any unhandled requests to stop them from being leaked
    if (!handled)
        delete req;
}

int Server::parseConfigFile() {
    ifstream file;
    // TODO: move config files to /etc as a standard, and add option to pass
    // config file location into the binary as a parameter
    file.open("/Users/Conor/Documents/Projects/frostbite/srv/.fconfig");
    if (!file) {
        return 0;
    }
    else {
        // read whole file into a string
        std::string s((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
        
        // create the JSON document
        rapidjson::Document document;
        document.Parse(s.c_str());
        
        // get port details
        if (!document.HasMember("port") || !document["port"].IsInt()) {
            cout << "ERROR: Config file: member \"port\" missing or " <<
                "non-int value" << endl;
            return 0;
        }
        this->port = document["port"].GetInt();
        
        if (!document.HasMember("hosts") || !document["hosts"].IsArray()) {
            cout << "ERROR: Config file: member \"hosts\" missing or " <<
                "non-array value" << endl;
            return 0;
        }
        
        for (rapidjson::SizeType i = 0; i < document["hosts"].Size(); i++) {
            if (!document["hosts"][i].HasMember("hostnames")) {
                cout << "ERROR: Config file: member \"hostnames\" missing " <<
                    "for host number " << i << endl;
                return 0;
            }
            if (!document["hosts"][i].HasMember("location")) {
                cout << "ERROR: Config file: member \"location\" missing " <<
                "for host number " << i << endl;
                return 0;
            }
            
            // this means that the host goes by multiple hostnames
            
            // this is the list of hostnames for this host
            std::vector<std::string> h;
            // this is the location for this hostname
            std::string l;
            // this is the amount of concurrent connections allowed for this
            // host.
            int c = DEFAULT_CONNECTIONS;
            
            if (document["hosts"][i]["hostnames"].IsArray()) {
                for (rapidjson::SizeType j = 0;
                     j < document["hosts"][i]["hostnames"].Size(); j++) {
                    
                    if (!document["hosts"][i]["hostnames"][j].IsString()) {
                        cout << "ERROR: Config file: expected \"hostnames\" " <<
                            "in \"hosts\" to be of type String or array of " <<
                            "Strings" << endl;
                        return 0;
                    }
                    h.push_back(document["hosts"][i]["hostnames"][j]
                                .GetString());
                }
            }
            else if (document["hosts"][i]["hostnames"].IsString()) {
                h.push_back(document["hosts"][i]["hostnames"].GetString());
            }
            else {
                cout << "ERROR: Config file: expected \"hostnames\" " <<
                    "in \"hosts\" to be of type String or array of " <<
                    "Strings for host number " << i << endl;
                return 0;
            }
            
            if (document["hosts"][i]["location"].IsString()) {
                l = document["hosts"][i]["location"].GetString();
                // force the location to end in a slash, for consistency
                if (l.back() != '/')
                    l += '/';
            }
            else {
                cout << "ERROR: Config file: expected \"location\" " <<
                    "in \"hosts\" to be of type String for host number " <<
                    i << endl;
                return 0;
            }
            
            
            if (document["hosts"][i].HasMember("connections") &&
                    document["hosts"][i]["connections"].IsInt()) {
                c = document["hosts"][i]["connections"].GetInt();
            }
            
            Hostname *hs = new Hostname(h);
            hosts.push_back(new Host(hs, l, c));
        }
    }
    file.close();
    return 1;
}

void Server::initListen(int sockfd) {
    int newsockfd;
    long n;
    char buffer[RECBUF];
    socklen_t clilen;
    sockaddr_in cli_addr;
    
    ::listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    bzero(buffer,256);
    
    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            Utils::error("ERROR on accept");
        bzero(buffer,256);
        n = read(newsockfd,buffer,RECBUF-1);
        if (n < 0)
            Utils::error("ERROR reading from socket");
        else if (n == RECBUF-1)
            printf("\n\n\nBUFFER OVERFLOW\n\n\n");
        
        // silence SIGPIPE errors from being thrown when writing to this socket.
        // this allows frostbite to easily discard broken sockets without any
        // unnecessary error handling
        int s = 1;
        setsockopt(newsockfd, SOL_SOCKET, SO_NOSIGPIPE, (void*)&s, sizeof(int));
        
        Request *req = new Request(string(buffer));
        dispatch(req, newsockfd);
    }
}

void Server::initServer() {
    // init the recieving socket and server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        Utils::error("ERROR opening socket");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        Utils::error("ERROR opening socket");

    // set the server configs
    sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    // silence SIGPIPE
    int s = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_NOSIGPIPE, (void*)&s, sizeof(int));
    // ignore binding errors
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void*)&s, sizeof(int));
    
    // try to bind to the port
    if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        Utils::error("ERROR on binding");
    
    cout << "frostbite server listening on port " << this->port << endl;
    
    // begin server listening
    initListen(sockfd);
    
    // close the server socket after listening is complete
    close(sockfd);
}

void Server::listen() {
    initServer();
}

Server::Server() {
    this->parseStatus = parseConfigFile();
}

Server *Server::getInstance() {
    if (instance == NULL) {
        instance = new Server();
    }
    return instance;
}

int Server::getParseStatus() {
    return this->parseStatus;
}