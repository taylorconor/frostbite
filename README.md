# frostbite

frostbite is a tiny pre-pre-alpha HTTP server written in C++. planned features include:

  - full HTTP standard compliance
  - connection pooling
  - PHP interpreter
  - [RapidJSON] for JSON config file parsing

### Config file

An example config file (.fconfig) for frostbite configuration is as follows
```javascript
{
    "port": 1234,
    "hosts": [
        {
            "hostnames": ["localhost", "www.localhost"],
            "location": "/Users/Conor/Documents/Projects/frostbite/srv",
            "connections": 64
        }
    ]
}


#### Version
0.0.1

[RapidJSON]:https://github.com/miloyip/rapidjson
