#pragma once

#include <string>
#include <unordered_map>
#include <raylib.h>

class assetManager
{
private:
    std::unordered_map<std::string, Texture2D> textures;
public:
    assetManager();
    ~assetManager();

    void init(const std::string& dirPath = "../assets");
    Texture2D load(const std::string& texName);
    void cleanup();
};
