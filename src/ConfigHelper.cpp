#include "../includes/ConfigHelper.h"
#include <windows.h>
#include <stdio.h>

ConfigHelper::ConfigHelper(const std::string& iniPath) : path(iniPath) {}

bool ConfigHelper::Exists() {
    DWORD attr;

    attr = GetFileAttributesA(path.c_str());
    return (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY));
}

void ConfigHelper::StoreComment(const char* sec, const char* key, const char* comment) {
    std::string mapKey;

    if (!comment || comment[0] == '\0') return;
    mapKey = std::string(sec) + "|" + key;
    m_comments[mapKey] = comment;
}

int ConfigHelper::ReadInt(const char* sec, const char* key, int def) {
    return GetPrivateProfileIntA(sec, key, def, path.c_str());
}

float ConfigHelper::ReadFloat(const char* sec, const char* key, float def) {
    char buf[64];

    if (GetPrivateProfileStringA(sec, key, "", buf, sizeof(buf), path.c_str()) > 0) {
        return (float)atof(buf);
    }
    return def;
}

void ConfigHelper::WriteInt(const char* sec, const char* key, int val, const char* comment) {
    char buf[512];

    StoreComment(sec, key, comment);
    sprintf_s(buf, "%d", val);
    WritePrivateProfileStringA(sec, key, buf, path.c_str());
}

void ConfigHelper::WriteFloat(const char* sec, const char* key, float val, const char* comment) {
    char buf[512];

    StoreComment(sec, key, comment);
    sprintf_s(buf, "%.2f", val);
    WritePrivateProfileStringA(sec, key, buf, path.c_str());
}

void ConfigHelper::Format(const std::string& headerComments) {
    FILE* f;
    bool firstSection;
    char line[512];
    size_t eqPos, end;
    std::string content, l, currentSection, mapKey, keyName;
    std::map<std::string, std::string>::iterator it;

    f = nullptr;
    fopen_s(&f, path.c_str(), "r");
    if (!f) return;

    if (!headerComments.empty()) {
        content += headerComments;
        if (headerComments.back() != '\n') {
            content += "\n";
        }
    }

    firstSection = true;
    while (fgets(line, sizeof(line), f)) {
        l = line;
        if (l[0] == ';' || l[0] == '#') {
            continue;
        }
        if (l[0] == '[') {
            if (!firstSection) {
                content += "\n";
            }
            firstSection = false;
            end = l.find(']');
            if (end != std::string::npos) {
                currentSection = l.substr(1, end - 1);
            }
            content += l;
            continue;
        }
        eqPos = l.find('=');
        if (eqPos != std::string::npos && !currentSection.empty()) {
            keyName = l.substr(0, eqPos);
            while (!keyName.empty() && isspace((unsigned char)keyName.back())) {
                keyName.pop_back();
            }
            mapKey = currentSection + "|" + keyName;
            it = m_comments.find(mapKey);
            if (it != m_comments.end() && !it->second.empty()) {
                content += "; " + it->second + "\n";
            }
        }
        content += l;
    }
    fclose(f);

    f = nullptr;
    fopen_s(&f, path.c_str(), "w");
    if (f) {
        fputs(content.c_str(), f);
        fclose(f);
    }
}
