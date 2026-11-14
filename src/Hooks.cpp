#include "../includes/Hooks.h"
#include "../includes/HeightBlip.h"
#include "../includes/HeightBlipConfig.h"

using namespace plugin;

void Hooks::InitHooksSA() {
    config.GenerateDefault();
    config.Read();

    // Hook the 6 call addresses in CRadar::ShowRadarTraceWithHeight (0x584070)
    // 1. Target at same altitude level (height == 2):
    patch::ReplaceFunctionCall(0x584164, HeightBlip::DrawRect_Normal_Border);
    patch::ReplaceFunctionCall(0x5841E9, HeightBlip::DrawRect_Normal_Inner);

    // 2. Target at lower altitude (height == 1, loc_5841F8):
    patch::ReplaceFunctionCall(0x584299, HeightBlip::Draw2DPolygon_Low_Border);
    patch::ReplaceFunctionCall(0x584328, HeightBlip::Draw2DPolygon_Low_Inner);

    // 3. Target at higher altitude (height == 0, loc_584337):
    patch::ReplaceFunctionCall(0x5843DF, HeightBlip::Draw2DPolygon_High_Border);
    patch::ReplaceFunctionCall(0x584471, HeightBlip::Draw2DPolygon_High_Inner);

    // Texture lifecycle events
    Events::initRwEvent += HeightBlip::Initialise;
    Events::shutdownRwEvent += HeightBlip::Shutdown;
}
