//
//  Hostname.cpp
//  redskin
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//
//  This class handles all of the ugly bits of hostname storage and testing,
//  since a host can have multiple hostnames, this handles all of them.
//

#include "Hostname.h"

// check if the test string is valid for this hostname
// TODO: implement this function, complete with smart string parsing
bool Hostname::contains(std::string test) {
    return true;
}

Hostname::Hostname(std::vector<std::string> hostnames) {
    this->hostnames = hostnames;
}