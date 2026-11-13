#include "../includes/HeightBlipConfig.h"
#include "../includes/ConfigHelper.h"
#include <plugin.h>

#define HEIGHTBLIP_INI_PATH PLUGIN_PATH("EditHeightBlip.ini")

HeightBlipConfig config;

HeightBlipConfig::HeightBlipConfig() {
    Enable = 1;
    BlipScale = 1.0f;
    DrawOutline = 0;
}

void HeightBlipConfig::Read() {
    std::string iniPath;

    iniPath = HEIGHTBLIP_INI_PATH;
    ConfigHelper cfg(iniPath);

    if (!cfg.Exists()) {
        return;
    }

    Enable = cfg.ReadInt("Main", "Enable", Enable);
    BlipScale = cfg.ReadFloat("Main", "BlipScale", BlipScale);
    DrawOutline = cfg.ReadInt("Main", "DrawOutline", DrawOutline);
}

void HeightBlipConfig::GenerateDefault() {
    std::string iniPath;

    iniPath = HEIGHTBLIP_INI_PATH;
    ConfigHelper cfg(iniPath);

    if (!cfg.Exists()) {
        cfg.WriteInt("Main", "Enable", Enable, "1: Enable SVG Height Blips, 0: Use Vanilla blips");
        cfg.WriteFloat("Main", "BlipScale", BlipScale, "Multiplier for blip size (default: 1.0)");
        cfg.WriteInt("Main", "DrawOutline", DrawOutline, "1: Draw black border pass around SVG blips, 0: Disable border pass");
        cfg.Format("; EditHeightBlip.ini - Configuration file for HeightBlip plugin\n");
    }
}
