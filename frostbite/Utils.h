//
//  Utils.h
//  frostbite
//
//  Created by Conor Taylor on 14/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__Utils__
#define __frostbite__Utils__

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <map>

class Utils {
public:
    static void error(const char *);
    
    static void dump_map(const std::map<std::string, std::string>& map);
    
    static std::string& trim(std::string &, const std::string &);
    
    static std::vector<std::string> explode(std::string const &, char);
    static std::vector<std::string> explode(std::string const &, char, int);
};

#endif /* defined(__frostbite__Utils__) */