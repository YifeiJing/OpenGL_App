#include <iostream>
#include <vector>

#include <dirent.h>

#include "asset.hpp"

const std::vector<std::string> getDirContents(const std::string& dir_name) {
    DIR* dirptr = opendir(dir_name.c_str());
    struct dirent* direntptr = readdir(dirptr);
    std::vector<std::string> res;
    while(direntptr) {
        res.push_back(direntptr->d_name);
        direntptr = readdir(dirptr);
    }
    closedir(dirptr);
    return res;
}

