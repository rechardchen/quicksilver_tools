#pragma once
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <mutex>
#include <windows.h>

typedef DWORD AddressType;  //TODO use AddressType other than DWORD
#define MAX_CALLSTACK_DEPTH 40
#define POOL_MASK 0xFFFF

//keep sync with QSEngine
enum MemoryOp
{
    E_MOP_Alloc, 
    E_MOP_Free,
    E_MOP_Register,
    E_MOP_SaveBin,
};
#pragma pack(1)
struct QSAllocateInfo// Alloc/Free
{
    DWORD					BaseAddress; //32bit-address
    DWORD					Count;
    DWORD					BackTrace[MAX_CALLSTACK_DEPTH];		//call stack address information
    DWORD                   CRC;								//CRC of BackTrace, used for output and grouping allocations
};

struct QSMemRegisterInfo
{
    DWORD                   handle;
};
struct QSMemoryPkg
{
    DWORD                   OpType;
    union
    {
        QSAllocateInfo         AllocInfo;
        QSMemRegisterInfo   RegisterInfo;
    };
};
#pragma pack()


struct QSMemoryState
{
    int64_t TotalAllocSize = 0;
    int64_t NumAllocs = 0; //current non-free allocations
    int64_t PeakAllocSize = 0;

    std::unordered_map<DWORD, QSAllocateInfo> AllocDB;  //TODO: use uniform integer hashing https://gist.github.com/badboy/6267743
};

class QSMemoryProfiler
{
public:
    //QSMemoryProfiler(const char* name):mCurIdx(0) { StartServer(name); }
    QSMemoryProfiler(const char* name);
    //~QSMemoryProfiler() { StopServer(); }
    ~QSMemoryProfiler();

    //void Process();
    void Serve();

    float           GetTotalAllocSizeMBytes() const;
    float           GetPeakAllocSizeMBytes() const;
    int64_t         GetNumAllocs() const;
    void            GetStats(float& total, float& peak, int64_t& num) const;
    QSMemoryState   GetMemStateCopy() const;

    std::string     SaveBinaryAllocData(float&total, int64_t& num) const;
    std::string     GetFunctionName(DWORD address);
    void            SaveBadFrees();

private:
    //void StartServer(const char* name);
    //void StopServer();
    void ProcessPkg(void* data);

    void InitStackWalking();
    void LoadProcessModules();

private:
    //class RemoteProfileServer* mServer = nullptr;
    class SyncRemoteProfileServer*  mServer = nullptr;
    QSMemoryState                   mMemState;
    mutable std::mutex              mMemStateMutex;
    HANDLE                          mGameProcess;
    bool                            mGStackWalkingInitialized;

    mutable std::mutex              mBadFreeMutex;
    int                             mBadFreeIndex = 0;
    static const int                BAD_FREE_RECORD_MAX = 200;
    DWORD                           mBadFreeSize[BAD_FREE_RECORD_MAX];
    DWORD                           mBadFrees[BAD_FREE_RECORD_MAX][MAX_CALLSTACK_DEPTH];
};

class QSMemoryView
{
public:
    QSMemoryView(QSMemoryProfiler& profiler);

    void CheckStats(float time);
    void ImDraw(float time);
    void SetVisible(bool flag);
    bool GetVisible() const     { return mVisible; }
    void ToggleVisible()        { mVisible = !mVisible; }

private:
    QSMemoryProfiler&     mProfilerRef;
    bool                        mVisible;

    float                       mLastCheckT;
    float                       mCurTotalAlloc;
    float                       mCurPeakAlloc;
    int64_t                    mCurAllocNum;

    static const int            MemRecordNum = 240;
    float                       mTotalMemRecord[MemRecordNum]; //0.5s a record
    int                         mRecordInsertIdx;

    //Dump
    struct DumpRecord
    {
        std::string DumpName;
        float Time = 0.f;
        float Total = 0.f;
        int64_t num = 0;
        bool selected = false;
    };
    std::vector<DumpRecord>     mDumpRecords;
    
private:
    //TreeView
    bool                        mShowDumpView = false;
    bool                        mShowDiffView = false;
    struct TreeNode*            mTreeRoot = nullptr;
    struct TreeNode*            mDiffRoot = nullptr;
    bool                        mTopDown = true;
    //std::string                 mCurDump;

    static void FreeTreeNode(TreeNode*);
    void        DumpView(const std::string& File);
    void        DiffDump(const std::string& File1, const std::string& File2);
};
