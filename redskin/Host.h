//
//  Host.h
//  redskin
//
//  Created by Conor Taylor on 15/11/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#ifndef __redskin__Host__
#define __redskin__Host__

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

#endif /* defined(__redskin__Host__) */
