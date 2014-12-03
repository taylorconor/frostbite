//
//  URI.cpp
//  frostbite
//
//  Created by Conor Taylor on 03/12/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "URI.h"

std::string URI::src() {
    return this->source;
}

void URI::configure() {
    this->fileStatus = stat(source.c_str(), &s);
}

URI::URI(std::string source) {
    this->source = source;
    this->configure();
}

void URI::append(std::string suffix) {
    this->source += suffix;
    // must reconfigure after redefining the uri
    this->configure();
}

bool URI::isValid() {
    return (this->fileStatus == 0 || (!this->isDirectory() && !this->isFile()));
}

bool URI::isDirectory() {
    return (s.st_mode & S_IFDIR);
}

bool URI::isFile() {
    return (s.st_mode & S_IFREG);
}

bool URI::isEmpty() {
    return (this->source.length() == 0);
}