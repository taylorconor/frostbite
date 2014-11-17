//
//  Hostname.h
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__Hostname__
#define __frostbite__Hostname__

#include <stdio.h>
#include <vector>
#include <string>

class Hostname {
private:
    std::vector<std::string> hostnames;
public:
    Hostname();
    Hostname(std::vector<std::string>);
    
    bool contains(std::string);
};

#endif /* defined(__frostbite__Hostname__) */
