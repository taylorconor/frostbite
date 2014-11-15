//
//  Host.h
//  frostbite
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __frostbite__Host__
#define __frostbite__Host__

#include <stdio.h>
#include <string>

#include "Hostname.h"
#include "Request.h"
#include "Response.h"
#include "Utils.h"

class Host {
private:
    Hostname hostname;
public:
    Host(Hostname);
    Hostname getHostname();
    void handleRequest(Request, int);
};

#endif /* defined(__frostbite__Host__) */
