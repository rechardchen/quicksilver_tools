#pragma once
#include <imgui.h>

namespace ImGui
{
    struct ColumnHeader
    {
        const char* label = nullptr;
        float size = -1.f;
        float sizeMIN = -1.f;
        bool allowResize = true;
    };

    //Draw column headers
    IMGUI_API void ColumnHeaders(const char* columnsId, ColumnHeader* headers, int count, bool border = true);

    //Synchronize with column headers
    IMGUI_API void BeginColumnHeadersSync(const char* columnsId, ColumnHeader* headers, int count, bool border = true);
    IMGUI_API void EndColumnHeadersSync(ColumnHeader* headers, int count);
}
