#pragma once
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

class Utils{
public:

    static std::string ReadTextFile(const char* path){
        std::ifstream file(path);
        if (!file) {
            throw std::runtime_error(std::string("无法打开着色器文件: ") + path);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
};