#include "assetManager.hpp"

#include <filesystem>
#include <iostream>

assetManager::assetManager() = default;

assetManager::~assetManager()
{
    cleanup();
}

void assetManager::init(const std::string& dirPath)
{
    if (!std::filesystem::exists(dirPath) || !std::filesystem::is_directory(dirPath)) {
        std::cerr << "assetmanager: directory not found: " << dirPath << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();

            if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
                std::string key = entry.path().stem().string();
                std::string fullPath = entry.path().string();

                Texture2D tex = LoadTexture(fullPath.c_str());

                if (tex.id != 0) {
                    textures[key] = tex;
                    std::cout << "success loading texture: " << key << std::endl;
                } else {
                    std::cerr << "failed to load texture: " << key << std::endl;
                }
            }
        }
    }
}

Texture2D assetManager::load(const std::string& texName)
{
    const auto tex = textures.find(texName);
    if (tex != textures.end()) {
        return tex->second;
    }

    std::cerr << "asset not found: " << texName << std::endl;
    return Texture2D{0};
}

void assetManager::cleanup()
{
    for (auto& pair : textures) {
        if (pair.second.id != 0) {
            UnloadTexture(pair.second);
        }
    }
    textures.clear();
}
