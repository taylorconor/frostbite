//
//  Hostname.cpp
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//
//  This class handles all of the ugly bits of hostname storage and testing,
//  since a host can have multiple hostnames, this handles all of them.
//

#include "Hostname.h"

// check if the test string is valid for this hostname
bool Hostname::contains(std::string test) {
    for (int i = 0; i < hostnames.size(); i++) {
        if (test.compare(0, hostnames[i].length(), hostnames[i]) == 0)
            return true;
    }
    return false;
}

Hostname::Hostname() {}
Hostname::Hostname(std::vector<std::string> hostnames) {
    this->hostnames = hostnames;
}