#pragma once
#include <stdint.h>
#include <vector>

#pragma pack(1)
enum ChunkOp
{
    E_COP_INIT,
    E_COP_CHANGE,
};

struct ChunkInit
{
    uint32_t NumChunk;
    uint32_t SizeChunk;
};
struct ChunkChange
{
    uint32_t Id;
    uint32_t ElementCount;
    uint32_t MaxElements;
    uint32_t AllocSize;
};

struct QSChunkPkg
{
    ChunkOp op;
    union
    {
        ChunkInit init;
        ChunkChange change;
    };
};

#pragma pack()

class QSSmallPoolProfiler
{
    friend class QSSmallPoolView;

public:
    QSSmallPoolProfiler(const char* name);
    ~QSSmallPoolProfiler();
    void Serve();


private:
    void ProcessPkg(void* data);

    std::vector<ChunkChange> mChunkData;
    class SyncRemoteProfileServer* mServer = nullptr;
};

class QSSmallPoolView
{
public:
    QSSmallPoolView(QSSmallPoolProfiler& profiler) : mProfiler(profiler) {}

    void ImDraw();
    void ToggleVisible() { mVisible = !mVisible; }

private:
    bool mVisible = false;
    QSSmallPoolProfiler& mProfiler;
};