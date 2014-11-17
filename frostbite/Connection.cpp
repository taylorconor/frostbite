//
//  Connection.cpp
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "Connection.h"

Connection::Connection(int sockfd) {
    this->sockfd = sockfd;
}