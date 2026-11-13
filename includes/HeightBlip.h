#pragma once
#include <plugin.h>
#include <CRect.h>
#include <CRGBA.h>
#include <CSprite2d.h>

enum eHeightBlipIndex {
    BLIP_HIGHER = 0,
    BLIP_LEVEL  = 1,
    BLIP_LOWER  = 2,
    NUM_HEIGHT_BLIPS = 3
};

class HeightBlip {
public:
    static CSprite2d Sprites[NUM_HEIGHT_BLIPS];
    static bool Loaded;

    static void Initialise();
    static void Shutdown();

    // Hook callbacks for CRadar::ShowRadarTraceWithHeight (0x584070)
    // Case RADAR_TRACE_HIGH (0x5841F8)
    static void Draw2DPolygon_High_Border(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color);
    static void Draw2DPolygon_High_Inner(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color);

    // Case RADAR_TRACE_NORMAL (0x5840E5)
    static void DrawRect_Normal_Border(const CRect& rect, const CRGBA& color);
    static void DrawRect_Normal_Inner(const CRect& rect, const CRGBA& color);

    // Case RADAR_TRACE_LOW (0x584337)
    static void Draw2DPolygon_Low_Border(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color);
    static void Draw2DPolygon_Low_Inner(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color);
};
