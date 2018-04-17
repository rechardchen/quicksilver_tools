#include "QSSmallPoolProfiler.h"
#include "Common/SyncRemoteProfileServer.h"
#include <imgui.h>

QSSmallPoolProfiler::QSSmallPoolProfiler(const char* name)
{
    mServer = new SyncRemoteProfileServer(name, sizeof(QSChunkPkg));
    mServer->RegisterPkgHandler(std::bind(&QSSmallPoolProfiler::ProcessPkg, this, std::placeholders::_1));
}

QSSmallPoolProfiler::~QSSmallPoolProfiler()
{
    if (mServer)
    {
        delete mServer;
        mServer = nullptr;
    }
}

void QSSmallPoolProfiler::Serve()
{
    mServer->ServeForEver();
}

void QSSmallPoolProfiler::ProcessPkg(void* data)
{
    const QSChunkPkg pkg = *(const QSChunkPkg*)data;
    switch (pkg.op)
    {
    case E_COP_INIT:
    {
        mChunkData.clear();
        for (int i = 0; i < pkg.init.NumChunk; ++i)
        {
            ChunkChange data;
            data.Id = i;
            data.AllocSize = data.ElementCount = data.MaxElements = 0;
            mChunkData.push_back(data);
        }
    }
        break;

    case E_COP_CHANGE:
    {
        auto id = pkg.change.Id;
        mChunkData[id] = pkg.change;
    }
        break;
    }
}

void QSSmallPoolView::ImDraw()
{
    if (!mVisible)
    {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(700, 400));
    if (!ImGui::Begin("Small Pool Profiler", &mVisible))
    {
        ImGui::End();
        return;
    }

    ImDrawList* drawList = ImGui::GetWindowDrawList();

    const float spacing = 1.f;
    const float sz = 6.f;
    const ImVec2 p = ImGui::GetCursorScreenPos();
    const ImVec4 red(1.f, 0.f, 0.f, 1.f);
    const ImVec4 green(0.f, 1.f, 0.f, 1.f);
    float x = p.x + 4.f, y = p.y + 4.f;

    auto iter = mProfiler.mChunkData.begin();
    while (iter != mProfiler.mChunkData.end())
    {
        for (int i = 0; i < 50; ++i)
        {
            float ratio = 0.f;
            if (iter->AllocSize > 0 && iter->ElementCount >= 0 && iter->MaxElements > 0)
            {
                ratio = float(iter->ElementCount) / iter->MaxElements;
            }
            ImColor col32(ImVec4(ratio, 1 - ratio, 0.f, 1.f));
            drawList->AddRect(ImVec2(x, y), ImVec2(x + sz, y + sz), col32);
            ++iter;
            if (iter == mProfiler.mChunkData.end())
            {
                break;
            }
            x += sz + spacing;
        }
        x = p.x + 4.f;
        y += sz + spacing;
    }

    ImGui::End();
}
