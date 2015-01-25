//
//  URI.cpp
//  frostbite
//
//  Created by Conor Taylor on 03/12/2014.
//  Copyright (c) 2014 Frostplant. All rights reserved.
//

#include "URI.h"

std::map<std::string, std::string> URI::mime_map = {
    {".css", "text/css"},
    {".htm", "text/html"},
    {".html", "text/html"},
    {".xml", "text/xml"},
    {".java", "text/x-java-source, text/java"},
    {".md", "text/plain"},
    {".txt", "text/plain"},
    {".asc", "text/plain"},
    {".gif", "image/gif"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".png", "image/png"},
    {".mp3", "audio/mpeg"},
    {".m3u", "audio/mpeg-url"},
    {".mp4", "video/mp4"},
    {".ogv", "video/ogg"},
    {".flv", "video/x-flv"},
    {".mov", "video/quicktime"},
    {".swf", "application/x-shockwave-flash"},
    {".js", "application/javascript"},
    {".pdf", "application/pdf"},
    {".doc", "application/msword"},
    {".ogg", "application/x-ogg"},
    {".zip", "application/octet-stream"},
    {".exe", "application/octet-stream"},
    {".class", "application/octet-stream"},
    {".otf", "application/octet-stream"},
    {".ttf", "application/octet-stream"}
};

std::string URI::src() {
    return source;
}

std::string URI::ext() {
    return this->extension;
}

std::string URI::mime() {
    return this->mime_type;
}

std::string URI::cleanExt() {
    std::string e = this->extension;
    if (e == ".phtml" || e.compare(0, 4, ".php") == 0)
        return ".php";
    return e;
}

std::string URI::parentDir() {
    boost::filesystem::path p(this->source);
    std::string filename = p.filename().string();
    return this->source.substr(0, this->source.length() - filename.length());
}

std::string URI::processExt(std::string e) {
    boost::algorithm::to_lower(e);
    return e;
}

std::string URI::processMIME(std::string e) {
    // leave the mime type blank if it's not in the map. the caller will exclude
    // this field from the HTTP header.
    if (mime_map.count(e) == 0)
        return "";
    else
        return mime_map.at(e);
}

void URI::configure() {
    std::string::size_type index;
    index = source.find('?', 0);
    if(index != std::string::npos) {
        this->source = source.substr(0, index);
        this->args = source.substr(index + 1);
    }
    else {
        this->source = source;
        this->args = "";
    }
    this->fileStatus = stat(source.c_str(), &s);
    this->extension = processExt(boost::filesystem::extension(this->source));
    this->mime_type = processMIME(this->extension);
}

URI::URI(std::string source) {
    this->source = source;
    this->configure();
}

void URI::append(std::string suffix) {
    this->source += suffix;
    // must reconfigure after redefining the uri
    this->configure();
}

bool URI::isValid() {
    return (this->fileStatus == 0 || (!this->isDirectory() && !this->isFile()));
}

bool URI::isDirectory() {
    return (s.st_mode & S_IFDIR);
}

bool URI::isFile() {
    return (s.st_mode & S_IFREG);
}

bool URI::isEmpty() {
    return (this->source.length() == 0);
}