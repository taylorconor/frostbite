//
//  main.cpp
//  frostbite
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include <iostream>
#include "Server.h"

int main(int argc, const char * argv[]) {
    Server *s = Server::getInstance();
    if (s->getParseStatus())
        s->listen();
    return 0;
}
