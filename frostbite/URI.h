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
#include <map>
#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

class URI {
private:
    std::string source;
    std::string args;
    std::string extension;
    std::string mime_type;
    struct stat s;
    int file_status;
    static std::map<std::string, std::string> mime_map;
    
    void configure();
    std::string process_ext(std::string);
    std::string process_mime(std::string);
public:
    URI();
    URI(std::string);
    void append(std::string);
    bool is_valid();
    bool is_directory();
    bool is_file();
    bool is_empty();
    std::string src();
    std::string ext();
    std::string mime();
    std::string clean_ext();
    std::string parent_dir();
};

#endif /* defined(__frostbite__URI__) */
