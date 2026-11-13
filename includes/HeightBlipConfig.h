#pragma once

class HeightBlipConfig {
public:
    int Enable;
    float BlipScale;
    int DrawOutline;

    HeightBlipConfig();
    void Read();
    void GenerateDefault();
};

extern HeightBlipConfig config;
