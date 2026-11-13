#pragma once
#include <string>
#include <map>

class ConfigHelper {
    std::string path;
    std::map<std::string, std::string> m_comments;
    void StoreComment(const char* sec, const char* key, const char* comment);
public:
    ConfigHelper(const std::string& iniPath);

    int ReadInt(const char* sec, const char* key, int def);
    float ReadFloat(const char* sec, const char* key, float def);

    void WriteInt(const char* sec, const char* key, int val, const char* comment = nullptr);
    void WriteFloat(const char* sec, const char* key, float val, const char* comment = nullptr);

    bool Exists();
    void Format(const std::string& headerComments = "");
};
