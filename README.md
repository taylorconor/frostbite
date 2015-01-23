# frostbite

frostbite is a tiny pre-pre-alpha HTTP server written in C++. 

features include:

  - connection pooling
  - PHP interpreter
  - [RapidJSON] for JSON config file parsing
        
planned features:
          
  - full HTTP standard compliance


## Config file

an example config file (`.fconfig`) for frostbite configuration is as follows:
```javascript
{
    "port": 80,     // TCP port to listen on
    "hosts": [      // frostbite supports multiple hosts per server
        {
            "hostnames": ["test.com", "www.test.com"],  // hostnames for this host
            "location": "/var/www/html",                // root document directory
            "connections": 64                           // max concurrent connections
        }
    ]
}

## Version
0.0.1

[RapidJSON]:https://github.com/miloyip/rapidjson
