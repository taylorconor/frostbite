//
//  ProxyHost.h
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#ifndef __frostbite__ProxyHost__
#define __frostbite__ProxyHost__

#include <stdio.h>

#include "Host.h"
#include "ProxyConnection.h"

class ProxyHost : public Host {
public:
    void handleRequest(Request *, int) override;
    
    ProxyHost(int);
};

#endif /* defined(__frostbite__ProxyHost__) */
