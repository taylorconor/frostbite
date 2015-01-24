//
//  main.cpp
//  frostbite
//
//  Created by Conor Taylor on 13/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include <iostream>
#include "Server.h"

#define DEFAULT_CONFIG  "/etc/frostbite/.fconfig"

int main(int argc, const char * argv[]) {
    Server *s = Server::getInstance();
    s->setConfig(DEFAULT_CONFIG);
    s->listen();
    return 0;
}
