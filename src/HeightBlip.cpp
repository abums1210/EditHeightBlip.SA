#include "../includes/HeightBlip.h"
#include "../includes/HeightBlipConfig.h"
#include "../includes/DirectImageRasterizer.h"
#include <windows.h>

CSprite2d HeightBlip::Sprites[NUM_HEIGHT_BLIPS];
bool HeightBlip::Loaded = false;

// --- Helper Functions ---

static void LoadBlipTexture(const char* folderPath, const char* fileName, CSprite2d& sprite) {
    sprite.m_pTexture = DirectImageRasterizer::FindSVGinFolderPath(folderPath, fileName, 64, 64);
    if (sprite.m_pTexture) {
        RwTextureSetFilterMode(sprite.m_pTexture, rwFILTERLINEARMIPLINEAR);
    }
}

static void GetPolygonBoundingBox(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
                                 float& outMinX, float& outMinY, float& outMaxX, float& outMaxY) {
    outMinX = (x1 < x2 ? x1 : x2);
    if (x3 < outMinX) outMinX = x3;
    if (x4 < outMinX) outMinX = x4;

    outMaxX = (x1 > x2 ? x1 : x2);
    if (x3 > outMaxX) outMaxX = x3;
    if (x4 > outMaxX) outMaxX = x4;

    outMinY = (y1 < y2 ? y1 : y2);
    if (y3 < outMinY) outMinY = y3;
    if (y4 < outMinY) outMinY = y4;

    outMaxY = (y1 > y2 ? y1 : y2);
    if (y3 > outMaxY) outMaxY = y3;
    if (y4 > outMaxY) outMaxY = y4;
}

static void DrawBlipSprite(CSprite2d& sprite, float minX, float minY, float maxX, float maxY, const CRGBA& color) {
    float cx, cy, hw, hh, scale;

    scale = config.BlipScale > 0.01f ? config.BlipScale : 1.0f;
    cx = (minX + maxX) * 0.5f;
    cy = (minY + maxY) * 0.5f;
    hw = (maxX - minX) * 0.5f * scale;
    hh = (maxY - minY) * 0.5f * scale;

    sprite.Draw(CRect(cx - hw, cy - hh, cx + hw, cy + hh), color);
}

static void HandleDrawPolygon(CSprite2d& sprite, bool isBorder,
                             float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color) {
    float minX, minY, maxX, maxY;

    if (config.Enable && sprite.m_pTexture) {
        if (!isBorder || config.DrawOutline) {
            GetPolygonBoundingBox(x1, y1, x2, y2, x3, y3, x4, y4, minX, minY, maxX, maxY);
            DrawBlipSprite(sprite, minX, minY, maxX, maxY, color);
        }
    } else {
        CSprite2d::Draw2DPolygon(x1, y1, x2, y2, x3, y3, x4, y4, color);
    }
}

static void HandleDrawRect(CSprite2d& sprite, bool isBorder, const CRect& rect, const CRGBA& color) {
    if (config.Enable && sprite.m_pTexture) {
        if (!isBorder || config.DrawOutline) {
            DrawBlipSprite(sprite, rect.left, rect.top, rect.right, rect.bottom, color);
        }
    } else {
        CSprite2d::DrawRect(rect, color);
    }
}

// --- HeightBlip Implementation ---

void HeightBlip::Initialise() {
    char folderPath[MAX_PATH];
    DWORD attr;

    Shutdown();

    if (!config.Enable) {
        return;
    }

    if (!DirectImageRasterizer::IsLoaded()) {
        DirectImageRasterizer::EnsureLoaded();
    }

    strcpy_s(folderPath, sizeof(folderPath), PLUGIN_PATH("levels"));
    attr = GetFileAttributesA(folderPath);
    if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        strcpy_s(folderPath, sizeof(folderPath), GAME_PATH("scripts\\levels"));
        attr = GetFileAttributesA(folderPath);
    }
    if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        return;
    }

    LoadBlipTexture(folderPath, "radar_higher", Sprites[BLIP_HIGHER]);
    LoadBlipTexture(folderPath, "radar_level", Sprites[BLIP_LEVEL]);
    LoadBlipTexture(folderPath, "radar_lower", Sprites[BLIP_LOWER]);

    Loaded = true;
}

void HeightBlip::Shutdown() {
    int i;

    for (i = 0; i < NUM_HEIGHT_BLIPS; i++) {
        if (Sprites[i].m_pTexture) {
            RwTextureDestroy(Sprites[i].m_pTexture);
            Sprites[i].m_pTexture = nullptr;
        }
    }
    Loaded = false;
}

// 0x5843DF: Target Higher Border Call (height == 0, loc_584337)
void HeightBlip::Draw2DPolygon_High_Border(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color) {
    HandleDrawPolygon(Sprites[BLIP_HIGHER], true, x1, y1, x2, y2, x3, y3, x4, y4, color);
}

// 0x584471: Target Higher Inner Call (height == 0, loc_584337)
void HeightBlip::Draw2DPolygon_High_Inner(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color) {
    HandleDrawPolygon(Sprites[BLIP_HIGHER], false, x1, y1, x2, y2, x3, y3, x4, y4, color);
}

// 0x584164: Target Level Border Call (height == 2, loc_5840E5)
void HeightBlip::DrawRect_Normal_Border(const CRect& rect, const CRGBA& color) {
    HandleDrawRect(Sprites[BLIP_LEVEL], true, rect, color);
}

// 0x5841E9: Target Level Inner Call (height == 2, loc_5840E5)
void HeightBlip::DrawRect_Normal_Inner(const CRect& rect, const CRGBA& color) {
    HandleDrawRect(Sprites[BLIP_LEVEL], false, rect, color);
}

// 0x584299: Target Lower Border Call (height == 1, loc_5841F8)
void HeightBlip::Draw2DPolygon_Low_Border(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color) {
    HandleDrawPolygon(Sprites[BLIP_LOWER], true, x1, y1, x2, y2, x3, y3, x4, y4, color);
}

// 0x584328: Target Lower Inner Call (height == 1, loc_5841F8)
void HeightBlip::Draw2DPolygon_Low_Inner(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color) {
    HandleDrawPolygon(Sprites[BLIP_LOWER], false, x1, y1, x2, y2, x3, y3, x4, y4, color);
}
