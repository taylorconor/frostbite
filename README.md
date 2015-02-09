# frostbite

frostbite is a tiny pre-pre-alpha HTTP web and proxy server written in C++. 

features include:

- connection pooling
- PHP interpreter
- [RapidJSON] for JSON config file parsing
- HTTP proxy server

planned features:

- full HTTP standard compliance


## Config file

an example config file (`.fconfig`) for frostbite configuration is as follows:
```javascript
{
  "port": 80,    // TCP port to listen on
  "proxy": {     // optional proxy config (defaults to status=off if not present)
    // all=proxy all connections, others=proxy all connections not present in hosts
    "status": "others",
    "connections": 64,                            // max concurrent proxy conns
    "cache": "/var/www/cache"                     // location to store cache
  },
  "hosts": [      // frostbite supports multiple hosts per server
    {
      "hostnames": ["test.com", "www.test.com"],  // hostnames for this host
      "location": "/var/www/html",                // root document directory
      "connections": 64                           // max concurrent connections
    }
  ]
}
```


## Version
0.0.2

[RapidJSON]:https://github.com/miloyip/rapidjson
