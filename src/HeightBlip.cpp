#include "../includes/HeightBlip.h"
#include "../includes/HeightBlipConfig.h"
#include "../includes/DirectImageRasterizer.h"
#include <windows.h>

CSprite2d HeightBlip::Sprites[NUM_HEIGHT_BLIPS];
bool HeightBlip::Loaded = false;

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

    Sprites[BLIP_HIGHER].m_pTexture = DirectImageRasterizer::FindSVGinFolderPath(folderPath, "radar_higher", 64, 64);
    if (Sprites[BLIP_HIGHER].m_pTexture) {
        RwTextureSetFilterMode(Sprites[BLIP_HIGHER].m_pTexture, rwFILTERLINEARMIPLINEAR);
    }

    Sprites[BLIP_LEVEL].m_pTexture = DirectImageRasterizer::FindSVGinFolderPath(folderPath, "radar_level", 64, 64);
    if (Sprites[BLIP_LEVEL].m_pTexture) {
        RwTextureSetFilterMode(Sprites[BLIP_LEVEL].m_pTexture, rwFILTERLINEARMIPLINEAR);
    }

    Sprites[BLIP_LOWER].m_pTexture = DirectImageRasterizer::FindSVGinFolderPath(folderPath, "radar_lower", 64, 64);
    if (Sprites[BLIP_LOWER].m_pTexture) {
        RwTextureSetFilterMode(Sprites[BLIP_LOWER].m_pTexture, rwFILTERLINEARMIPLINEAR);
    }

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

// 0x584299: RADAR_TRACE_HIGH Border Call
void HeightBlip::Draw2DPolygon_High_Border(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color) {
    float minX, maxX, minY, maxY, cx, cy, hw, hh, scale;

    if (config.Enable && Sprites[BLIP_HIGHER].m_pTexture) {
        if (config.DrawOutline) {
            minX = (x1 < x2 ? x1 : x2);
            if (x3 < minX) minX = x3;
            if (x4 < minX) minX = x4;

            maxX = (x1 > x2 ? x1 : x2);
            if (x3 > maxX) maxX = x3;
            if (x4 > maxX) maxX = x4;

            minY = (y1 < y2 ? y1 : y2);
            if (y3 < minY) minY = y3;
            if (y4 < minY) minY = y4;

            maxY = (y1 > y2 ? y1 : y2);
            if (y3 > maxY) maxY = y3;
            if (y4 > maxY) maxY = y4;

            scale = config.BlipScale > 0.01f ? config.BlipScale : 1.0f;
            cx = (minX + maxX) * 0.5f;
            cy = (minY + maxY) * 0.5f;
            hw = (maxX - minX) * 0.5f * scale;
            hh = (maxY - minY) * 0.5f * scale;

            Sprites[BLIP_HIGHER].Draw(CRect(cx - hw, cy - hh, cx + hw, cy + hh), color);
        }
    } else {
        CSprite2d::Draw2DPolygon(x1, y1, x2, y2, x3, y3, x4, y4, color);
    }
}

// 0x584328: RADAR_TRACE_HIGH Inner Call
void HeightBlip::Draw2DPolygon_High_Inner(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color) {
    float minX, maxX, minY, maxY, cx, cy, hw, hh, scale;

    if (config.Enable && Sprites[BLIP_HIGHER].m_pTexture) {
        minX = (x1 < x2 ? x1 : x2);
        if (x3 < minX) minX = x3;
        if (x4 < minX) minX = x4;

        maxX = (x1 > x2 ? x1 : x2);
        if (x3 > maxX) maxX = x3;
        if (x4 > maxX) maxX = x4;

        minY = (y1 < y2 ? y1 : y2);
        if (y3 < minY) minY = y3;
        if (y4 < minY) minY = y4;

        maxY = (y1 > y2 ? y1 : y2);
        if (y3 > maxY) maxY = y3;
        if (y4 > maxY) maxY = y4;

        scale = config.BlipScale > 0.01f ? config.BlipScale : 1.0f;
        cx = (minX + maxX) * 0.5f;
        cy = (minY + maxY) * 0.5f;
        hw = (maxX - minX) * 0.5f * scale;
        hh = (maxY - minY) * 0.5f * scale;

        Sprites[BLIP_HIGHER].Draw(CRect(cx - hw, cy - hh, cx + hw, cy + hh), color);
    } else {
        CSprite2d::Draw2DPolygon(x1, y1, x2, y2, x3, y3, x4, y4, color);
    }
}

// 0x584164: RADAR_TRACE_NORMAL Border Call
void HeightBlip::DrawRect_Normal_Border(const CRect& rect, const CRGBA& color) {
    float cx, cy, hw, hh, scale;

    if (config.Enable && Sprites[BLIP_LEVEL].m_pTexture) {
        if (config.DrawOutline) {
            scale = config.BlipScale > 0.01f ? config.BlipScale : 1.0f;
            cx = (rect.left + rect.right) * 0.5f;
            cy = (rect.top + rect.bottom) * 0.5f;
            hw = (rect.right - rect.left) * 0.5f * scale;
            hh = (rect.bottom - rect.top) * 0.5f * scale;

            Sprites[BLIP_LEVEL].Draw(CRect(cx - hw, cy - hh, cx + hw, cy + hh), color);
        }
    } else {
        CSprite2d::DrawRect(rect, color);
    }
}

// 0x5841E9: RADAR_TRACE_NORMAL Inner Call
void HeightBlip::DrawRect_Normal_Inner(const CRect& rect, const CRGBA& color) {
    float cx, cy, hw, hh, scale;

    if (config.Enable && Sprites[BLIP_LEVEL].m_pTexture) {
        scale = config.BlipScale > 0.01f ? config.BlipScale : 1.0f;
        cx = (rect.left + rect.right) * 0.5f;
        cy = (rect.top + rect.bottom) * 0.5f;
        hw = (rect.right - rect.left) * 0.5f * scale;
        hh = (rect.bottom - rect.top) * 0.5f * scale;

        Sprites[BLIP_LEVEL].Draw(CRect(cx - hw, cy - hh, cx + hw, cy + hh), color);
    } else {
        CSprite2d::DrawRect(rect, color);
    }
}

// 0x5843DF: RADAR_TRACE_LOW Border Call
void HeightBlip::Draw2DPolygon_Low_Border(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color) {
    float minX, maxX, minY, maxY, cx, cy, hw, hh, scale;

    if (config.Enable && Sprites[BLIP_LOWER].m_pTexture) {
        if (config.DrawOutline) {
            minX = (x1 < x2 ? x1 : x2);
            if (x3 < minX) minX = x3;
            if (x4 < minX) minX = x4;

            maxX = (x1 > x2 ? x1 : x2);
            if (x3 > maxX) maxX = x3;
            if (x4 > maxX) maxX = x4;

            minY = (y1 < y2 ? y1 : y2);
            if (y3 < minY) minY = y3;
            if (y4 < minY) minY = y4;

            maxY = (y1 > y2 ? y1 : y2);
            if (y3 > maxY) maxY = y3;
            if (y4 > maxY) maxY = y4;

            scale = config.BlipScale > 0.01f ? config.BlipScale : 1.0f;
            cx = (minX + maxX) * 0.5f;
            cy = (minY + maxY) * 0.5f;
            hw = (maxX - minX) * 0.5f * scale;
            hh = (maxY - minY) * 0.5f * scale;

            Sprites[BLIP_LOWER].Draw(CRect(cx - hw, cy - hh, cx + hw, cy + hh), color);
        }
    } else {
        CSprite2d::Draw2DPolygon(x1, y1, x2, y2, x3, y3, x4, y4, color);
    }
}

// 0x584471: RADAR_TRACE_LOW Inner Call
void HeightBlip::Draw2DPolygon_Low_Inner(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, const CRGBA& color) {
    float minX, maxX, minY, maxY, cx, cy, hw, hh, scale;

    if (config.Enable && Sprites[BLIP_LOWER].m_pTexture) {
        minX = (x1 < x2 ? x1 : x2);
        if (x3 < minX) minX = x3;
        if (x4 < minX) minX = x4;

        maxX = (x1 > x2 ? x1 : x2);
        if (x3 > maxX) maxX = x3;
        if (x4 > maxX) maxX = x4;

        minY = (y1 < y2 ? y1 : y2);
        if (y3 < minY) minY = y3;
        if (y4 < minY) minY = y4;

        maxY = (y1 > y2 ? y1 : y2);
        if (y3 > maxY) maxY = y3;
        if (y4 > maxY) maxY = y4;

        scale = config.BlipScale > 0.01f ? config.BlipScale : 1.0f;
        cx = (minX + maxX) * 0.5f;
        cy = (minY + maxY) * 0.5f;
        hw = (maxX - minX) * 0.5f * scale;
        hh = (maxY - minY) * 0.5f * scale;

        Sprites[BLIP_LOWER].Draw(CRect(cx - hw, cy - hh, cx + hw, cy + hh), color);
    } else {
        CSprite2d::Draw2DPolygon(x1, y1, x2, y2, x3, y3, x4, y4, color);
    }
}
