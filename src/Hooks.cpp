#include "../includes/Hooks.h"
#include "../includes/HeightBlip.h"
#include "../includes/HeightBlipConfig.h"

using namespace plugin;

void Hooks::InitHooksSA() {
    config.GenerateDefault();
    config.Read();

    // Hook the 6 call addresses in CRadar::ShowRadarTraceWithHeight (0x584070)
    // 1. RADAR_TRACE_NORMAL (height == 2):
    patch::ReplaceFunctionCall(0x584164, HeightBlip::DrawRect_Normal_Border);
    patch::ReplaceFunctionCall(0x5841E9, HeightBlip::DrawRect_Normal_Inner);

    // 2. RADAR_TRACE_HIGH (height == 1):
    patch::ReplaceFunctionCall(0x584299, HeightBlip::Draw2DPolygon_High_Border);
    patch::ReplaceFunctionCall(0x584328, HeightBlip::Draw2DPolygon_High_Inner);

    // 3. RADAR_TRACE_LOW (height == 0):
    patch::ReplaceFunctionCall(0x5843DF, HeightBlip::Draw2DPolygon_Low_Border);
    patch::ReplaceFunctionCall(0x584471, HeightBlip::Draw2DPolygon_Low_Inner);

    // Texture lifecycle events
    Events::initRwEvent += HeightBlip::Initialise;
    Events::shutdownRwEvent += HeightBlip::Shutdown;
}
