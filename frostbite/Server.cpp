//
//  Server.cpp
//  frostbite
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Server.h"

Server *Server::_instance = nullptr;

// dispatch the request to the correct host for handling
void Server::dispatch(Request *req, int newsockfd) {
    bool handled = false;
    if (proxy.status == PROXY_ALL) {
        proxy.host->handle_request(req, newsockfd);
    }
    for (int i = 0; i < hosts.size() && !handled; i++) {
        Hostname *h = hosts[i]->hostname();
        if (h->contains(req->host())) {
            hosts[i]->handle_request(req, newsockfd);
            handled = true;
            break;
        }
    }
    // if no proxying is being used, delete any unhandled requests to stop them
    // from being leaked
    if (!handled) {
        if (proxy.status == PROXY_OTHERS)
            proxy.host->handle_request(req, newsockfd);
        else
            delete req;
    }
}

int Server::parse_config() {
    std::ifstream file;
    file.open(config);
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
            Utils::error("ERROR: Config file: member \"port\" missing or "
                         "non-int value");
            return 0;
        }
        port = document["port"].GetInt();
        
        if (document.HasMember("proxy") && document["proxy"].IsObject()) {
            if (document["proxy"].HasMember("status") &&
                document["proxy"]["status"].IsString()) {
                std::string p = document["proxy"]["status"].GetString();
                if (p.compare("off") == 0)
                    proxy.status = PROXY_OFF;
                else if (p.compare("others") == 0)
                    proxy.status = PROXY_OTHERS;
                else if (p.compare("all") == 0)
                    proxy.status = PROXY_ALL;
                else
                    proxy.status = PROXY_OFF;
            }
            else {
                proxy.status = PROXY_OFF;
            }
            
            if (document["proxy"].HasMember("connections") &&
                document["proxy"]["connections"].IsInt()) {
                proxy.connections = document["proxy"]["connections"].GetInt();
            }
            else {
                proxy.connections = MAX_PROXY_CONNS;
            }
            
            if (document["proxy"].HasMember("cache") &&
                document["proxy"]["cache"].IsString()) {
                std::string cache = document["proxy"]["cache"].GetString();
                if (cache.compare("off") == 0) {
                    proxy.should_cache = false;
                }
                else if (Utils::exists(cache)) {
                    proxy.should_cache = true;
                    proxy.cache = cache;
                }
                else {
                    Utils::error("WARNING: Config file: cache location does not"
                                 " exist or is not writeable");
                    proxy.should_cache = false;
                }
            }
            
            if (document["proxy"].HasMember("console") &&
                document["proxy"]["console"].IsString()) {
                std::string console = document["proxy"]["console"].GetString();
                if (Utils::exists(console)) {
                    proxy.console = console;
                }
                else {
                    Utils::error("WARNING: Proxy console: location does not "
                                 "exist or is not writeable");
                    proxy.console = nullptr;
                }
            }
        }
        else {
            proxy.status = PROXY_OFF;
        }
        
        if (!document.HasMember("hosts") || !document["hosts"].IsArray()) {
            Utils::error("ERROR: Config file: member \"hosts\" missing or "
                         "non-array value");
            return 0;
        }
        
        for (rapidjson::SizeType i = 0; i < document["hosts"].Size(); i++) {
            if (!document["hosts"][i].HasMember("hostnames")) {
                Utils::error("ERROR: Config file: member \"hostnames\" missing "
                             "for host number " + std::to_string(i));
                return 0;
            }
            if (!document["hosts"][i].HasMember("location")) {
                Utils::error("ERROR: Config file: member \"location\" missing "
                             "for host number " + std::to_string(i));
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
                        Utils::error("ERROR: Config file: expected "
                                     "\"hostnames\" in \"hosts\" to be of type"
                                     "String or array of String");
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
                Utils::error("ERROR: Config file: expected \"hostnames\" "
                             "in \"hosts\" to be of type String or array of "
                             "Strings for host number " + std::to_string(i));
                return 0;
            }
            
            if (document["hosts"][i]["location"].IsString()) {
                l = document["hosts"][i]["location"].GetString();
                // force the location to end in a slash, for consistency
                if (l.back() != '/')
                    l += '/';
            }
            else {
                Utils::error("ERROR: Config file: expected \"location\" "
                             "in \"hosts\" to be of type String for host "
                             "number " + std::to_string(i));
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

void Server::init_listen(int sockfd) {
    int newsockfd;
    long n;
    char buffer[RECBUF];
    socklen_t clilen;
    sockaddr_in cli_addr;
    
    ::listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    bzero(buffer, RECBUF);
    
    while (1) {
        newsockfd = -1;
        for (int i = 0; newsockfd < 0; i++) {
            newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
            if (i >= MAX_ACCEPT_ATTEMPTS && newsockfd < 0)
                Utils::error("ERROR on accept; failed "
                             +std::to_string(MAX_ACCEPT_ATTEMPTS)
                             +"attempts");
        }
        bzero(buffer, RECBUF);
        n = read(newsockfd,buffer,RECBUF-1);
        if (n < 0)
            Utils::error("ERROR reading from socket");
        else if (n == RECBUF-1)
            Utils::error("ERROR: Server init listen: buffer overflow");
        
        // silence SIGPIPE errors from being thrown when writing to this socket.
        // this allows frostbite to easily discard broken sockets without any
        // unnecessary error handling
        int s = 1;
        setsockopt(newsockfd, SOL_SOCKET, SO_NOSIGPIPE, (void*)&s, sizeof(int));
        
        Request *req = new Request(std::string(buffer));
        dispatch(req, newsockfd);
    }
}

void Server::init_server() {
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
    
    // initialise proxy host
    if (proxy.status != PROXY_OFF) {
        if (proxy.should_cache)
            proxy.host = new ProxyHost(proxy.connections,
                                       proxy.cache, proxy.console);
        else
            proxy.host = new ProxyHost(proxy.connections);
    }
    
    std::cout << "frostbite server listening on port " << port << std::endl;
    
    // begin server listening
    init_listen(sockfd);
    
    // close the server socket after listening is complete
    close(sockfd);
}

void Server::listen() {
    _parse_status = parse_config();
    if (_parse_status)
        init_server();
    else
        std::cout << "error parsing config file" << std::endl;
}

void Server::set_config(std::string config) {
    this->config = config;
}

Server::Server() {}
Server *Server::instance() {
    if (_instance == NULL) {
        _instance = new Server();
    }
    return _instance;
}

int Server::parse_status() {
    return _parse_status;
}