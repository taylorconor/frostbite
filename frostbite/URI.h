//
//  URI.h
//  frostbite
//
//  Created by Conor Taylor on 03/12/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__URI__
#define __frostbite__URI__

#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

class URI {
private:
    void configure();
    std::string source;
    std::string extension;
    struct stat s;
    int fileStatus;
    
    std::string processExt(std::string);
public:
    URI();
    URI(std::string);
    void append(std::string);
    bool isValid();
    bool isDirectory();
    bool isFile();
    bool isEmpty();
    std::string src();
    std::string ext();
    std::string cleanExt();
    std::string parentDir();
};

#endif /* defined(__frostbite__URI__) */