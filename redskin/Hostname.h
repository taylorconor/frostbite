//
//  Hostname.h
//  redskin
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __redskin__Hostname__
#define __redskin__Hostname__

#include <stdio.h>
#include <vector>
#include <string>

class Hostname {
private:
    std::vector<std::string> hostnames;
public:
    Hostname(std::vector<std::string>);
    bool contains(std::string);
};

#endif /* defined(__redskin__Hostname__) */
