//
//  ProxyConnection.h
//  frostbite
//
//  Created by Conor Taylor on 27/01/2015.
//  Copyright (c) 2015 Frostplant. All rights reserved.
//

#ifndef __frostbite__ProxyConnection__
#define __frostbite__ProxyConnection__

#include <stdio.h>
#include <curl/curl.h>

#include "Connection.h"
#include "Response.h"

class ProxyConnection : public Connection {
private:
public:
    static size_t writer(char *, size_t, size_t, void *);
    ProxyConnection(Request *, int);
    void handleConnection();
};

#endif /* defined(__frostbite__ProxyConnection__) */
