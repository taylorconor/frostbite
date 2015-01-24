//
//  URI.cpp
//  frostbite
//
//  Created by Conor Taylor on 03/12/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "URI.h"

std::string URI::src() {
    return source;
}

std::string URI::ext() {
    return this->extension;
}

std::string URI::cleanExt() {
    std::string e = this->extension;
    if (e == ".phtml" || e.compare(0, 4, ".php") == 0)
        return ".php";
    return e;
}

std::string URI::parentDir() {
    boost::filesystem::path p(this->source);
    std::string filename = p.filename().string();
    return this->source.substr(0, this->source.length() - filename.length());
}

std::string URI::processExt(std::string e) {
    boost::algorithm::to_lower(e);
    return e;
}

void URI::configure() {
    this->fileStatus = stat(source.c_str(), &s);
    this->extension = processExt(boost::filesystem::extension(this->source));
}

URI::URI(std::string source) {
    this->source = source;
    std::string::size_type index;
    index = source.find('?', 0);
    if(index != std::string::npos) {
        this->source = source.substr(0, index);
        this->args = source.substr(index + 1);
    }
    else {
        this->source = source;
        this->args = "";
    }
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