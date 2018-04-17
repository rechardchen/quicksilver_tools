#include "QSMemoryProfiler.h"
#include "common/SyncRemoteProfileServer.h"
#include <string>
#include <cassert>
#include <map>
#include <algorithm>
#include <fstream>
#include <array>
#include <Psapi.h>
#include <DbgHelp.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_column_headers.h>
#include <filesystem>

//#pragma optimize("",off)

namespace fs = std::experimental::filesystem;

namespace
{
    template <class T>
    void Serialize(std::ostream& os, T value)
    {
        os.write((const char*)&value, sizeof(T));
    }

    template <class T>
    void Serialize(std::istream& is, T& value)
    {
        is.read((char*)&value, sizeof(T));
    }

    void Serialize(std::ostream&ar, const std::string& str)
    {
        //wtf?
        unsigned int size = str.length();
        Serialize(ar, size);
        ar.write(str.c_str(), size);
    }

    void Serialize(std::istream&is, std::string& str)
    {
        unsigned int size = 0;
        Serialize(is, size);
        str.resize(size);
        is.read((char*)str.c_str(), size);
    }

    template<typename T1, typename T2>
    void Serialize(std::ostream &ar, std::pair<T1, T2>& value)
    {
        Serialize(ar, value.first);
        Serialize(ar, value.second);
    }

    template<typename T1, typename T2>
    void Serialize(std::istream&is, std::pair<T1,T2>& value)
    {
        Serialize(is, value.first);
        Serialize(is, value.second);
    }

    template<typename T1, typename T2>
    void Serialize(std::ostream& ar, std::map<T1, T2>& value)
    {
        int size = (int)value.size();
        Serialize(ar, size);
        for (auto it = value.begin(); it != value.end(); ++it)
        {
            Serialize(ar, *it);
        }
    }

    template <typename T1, typename T2>
    void Serialize(std::istream& is, std::map<T1,T2>& value)
    {
        int size = 0;
        Serialize(is, size);
        for (int i = 0;i < size; ++i)
        {
            std::pair<T1, T2> keyValue;
            Serialize(is, keyValue);
            value.insert(keyValue);
        }
    }
}
//void QSMemoryProfiler::StartServer(const char* name)
//{
//    StopServer();
//    std::string pipe = "\\\\.\\pipe\\" + std::string(name);
//    mServer = new RemoteProfileServer(pipe.c_str(), 1000*sizeof(QSMemoryPkg), 1024);
//}
//
//void QSMemoryProfiler::StopServer()
//{
//    if (mServer)
//    {
//        delete mServer;
//        mServer = nullptr;
//    }
//}

QSMemoryProfiler::QSMemoryProfiler(const char* name)
{
    mGameProcess = INVALID_HANDLE_VALUE;
    mGStackWalkingInitialized = false;
    mServer = new SyncRemoteProfileServer(name, sizeof(QSMemoryPkg));
    mServer->RegisterPkgHandler(std::bind(&QSMemoryProfiler::ProcessPkg, this, std::placeholders::_1));
    memset(&mBadFreeSize[0], 0, sizeof mBadFreeSize);
    memset(&mBadFrees[0][0], 0, MAX_CALLSTACK_DEPTH*BAD_FREE_RECORD_MAX * sizeof(DWORD));
}

QSMemoryProfiler::~QSMemoryProfiler()
{
    if (mServer)
    {
        delete mServer;
        mServer = nullptr;
    }
}

void QSMemoryProfiler::Serve()
{
    mServer->ServeForEver();
}

float QSMemoryProfiler::GetTotalAllocSizeMBytes() const
{
    std::lock_guard<std::mutex> MemStateLock(mMemStateMutex);
    return mMemState.TotalAllocSize / 1024.f / 1024.f;
}

float QSMemoryProfiler::GetPeakAllocSizeMBytes() const
{
    std::lock_guard<std::mutex> MemStateLock(mMemStateMutex);
    return mMemState.PeakAllocSize / 1024.f / 1024.f;
}

int64_t QSMemoryProfiler::GetNumAllocs() const
{
    std::lock_guard<std::mutex> MemStateLock(mMemStateMutex);
    return mMemState.NumAllocs;
}

void QSMemoryProfiler::GetStats(float& total, float& peak, int64_t& num) const
{
    std::lock_guard<std::mutex> MemStateLock(mMemStateMutex);
    total = mMemState.TotalAllocSize / 1024.f / 1024.f;
    peak = mMemState.PeakAllocSize / 1024.f / 1024.f;
    num = mMemState.NumAllocs;
}

//void QSMemoryProfiler::Process()
//{
//    if (mServer && mServer->Process())
//    {
//        auto pkg = mServer->GetPackage<QSMemoryPkg>();
//
//       /* char msg[256] = { 0 };
//        sprintf_s(msg, "[RemoteProfileServer] opType: %d, address: %d", pkg.OpType, pkg.AllocInfo.BaseAddress);
//        OutputDebugStringA(msg);
//
//        records.push_back(pkg.AllocInfo.BaseAddress);*/
//        assert(mCurIdx++ == pkg.Idx);
//        switch (pkg.OpType)
//        {
//        case E_MOP_Alloc:
//            {
//                std::lock_guard<std::mutex> memStateLock(mMemStateMutex);
//                auto& db = mMemState.AllocDB;
//                auto& info = pkg.AllocInfo;
//                auto iter = db.find(info.BaseAddress);
//                if (iter == db.end())
//                {
//                    db[info.BaseAddress] = info;
//                    mMemState.TotalAllocSize += info.Count;
//                    mMemState.NumAllocs += 1;
//                }
//                else  //maybe realloc
//                {
//                    mMemState.TotalAllocSize -= iter->second.Count;
//                    db[info.BaseAddress] = info;
//                    mMemState.TotalAllocSize += info.Count;
//                }
//
//                if (mMemState.TotalAllocSize > mMemState.PeakAllocSize)
//                {
//                    mMemState.PeakAllocSize = mMemState.TotalAllocSize;
//                }
//            }
//            break;
//
//        case E_MOP_Free:
//            {
//                std::lock_guard<std::mutex> memStateLock(mMemStateMutex);
//                auto& db = mMemState.AllocDB;
//                auto& info = pkg.AllocInfo;
//                auto iter = db.find(info.BaseAddress);
//                //assert(iter != db.end());
//                if (iter != db.end())
//                {
//                    auto& allocInfo = iter->second;
//                    //assert(allocInfo.Count == info.Count);
//                    db.erase(info.BaseAddress);
//                    mMemState.TotalAllocSize -= allocInfo.Count;
//                    mMemState.NumAllocs -= 1;       //should?
//                }
//                else
//                {
//                    //something currently out of control
//                    printf("some free out of our control happends!\n");
//                }
//            }
//            break;
//
//        default:
//            break;
//        }
//    }
//}

QSMemoryState QSMemoryProfiler::GetMemStateCopy() const
{
    std::lock_guard<std::mutex> memStateLock(mMemStateMutex);
    return mMemState;
}

struct QSGroupedAllocInfo
{
    DWORD CRC;
    DWORD BackTrace[MAX_CALLSTACK_DEPTH];                       //Notice: temporary address
    SIZE_T TotalSize;
    INT    Count;
    
    size_t ReadableSize;                    //not used
    size_t CompSize;                        //not used
    friend void Serialize(std::ostream&, const QSGroupedAllocInfo&);
    friend void Serialize(std::istream&, QSGroupedAllocInfo&);
};

void Serialize(std::ostream& os, const QSGroupedAllocInfo& alloc)
{
    os.write((char*)alloc.BackTrace, MAX_CALLSTACK_DEPTH * sizeof(DWORD));
    Serialize(os, alloc.TotalSize);
    Serialize(os, alloc.Count);
    Serialize(os, alloc.ReadableSize);
    Serialize(os, alloc.CompSize);
    Serialize(os, alloc.CRC);
}

void Serialize(std::istream& is, QSGroupedAllocInfo& alloc)
{
    is.read((char*)alloc.BackTrace, MAX_CALLSTACK_DEPTH * sizeof(DWORD));
    Serialize(is, alloc.TotalSize);
    Serialize(is, alloc.Count);
    Serialize(is, alloc.ReadableSize);
    Serialize(is, alloc.CompSize);
    Serialize(is, alloc.CRC);
}



std::string QSMemoryProfiler::SaveBinaryAllocData(float&total, int64_t& num) const
{
    
    std::map<DWORD, QSGroupedAllocInfo> GroupedAllocations;
    //std::vector<DWORD> addressArray;

    {
        std::lock_guard<std::mutex> MemStatLock(mMemStateMutex);
        total = mMemState.TotalAllocSize / 1024.f;
        num = mMemState.NumAllocs;
        auto &db = mMemState.AllocDB;
        for (auto it=db.begin(); it != db.end(); ++it)
        {
            auto groupedIter = GroupedAllocations.find(it->second.CRC);
            if (groupedIter == GroupedAllocations.end())
            {
                QSGroupedAllocInfo &newInfo = GroupedAllocations[it->second.CRC];
                newInfo.CRC = it->second.CRC;
                memcpy(newInfo.BackTrace, it->second.BackTrace, sizeof(DWORD)*MAX_CALLSTACK_DEPTH);
                newInfo.TotalSize = it->second.Count;
                newInfo.Count = 1;
            }
            else
            {
                groupedIter->second.TotalSize += it->second.Count;
                groupedIter->second.Count += 1;
            }
        }
    }
  /*  auto& db = mMemState.AllocDB;
    for (auto it = db.begin(); it != db.end(); ++it)
    {
        auto groupedIter = GroupedAllocations.find(it->second.CRC);
        if (groupedIter == GroupedAllocations.end())
        {
            QSGroupedAllocInfo newInfo;
            newInfo.CRC = it->second.CRC;
            newInfo.BackTrace = it->second.BackTrace;
            newInfo.TotalSize = it->second.Count;
            newInfo.Count = 1;

            GroupedAllocations[newInfo.CRC] = newInfo;
            for (auto address: it->second.BackTrace)
            {
                if (0 == address)
                {
                    break;
                }
                if (std::find(addressArray.begin(), addressArray.end(), address) == addressArray.end())
                {
                    addressArray.push_back(address);
                }
            }
        }
        else
        {
            groupedIter->second.TotalSize += it->second.Count;
            groupedIter->second.Count += 1;
        }
    }

    std::map<DWORD, std::string> funcNameMap;
    std::sort(addressArray.begin(), addressArray.end(), [](DWORD a, DWORD b) { return a > b; });
    for (auto address: addressArray)
    {
        std::string funcName = GetFunctionName(address);
        funcNameMap[address] = funcName;
    }*/

    //dump
    char fileName[256] = { 0 };
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        sprintf_s(fileName, "MemSave_%d%02d%02d_%02d%02d%02d.dat", (int)st.wYear, (int)st.wMonth, (int)st.wDay,
            (int)st.wHour, (int)st.wMinute, (int)st.wSecond);
    }
    std::ofstream outFile;
    outFile.open(fileName, std::ios::binary | std::ios::out);
    Serialize(outFile ,GroupedAllocations);
    //outFile << GroupedAllocations;
    //Serialize(outFile, funcNameMap);
    //outFile << funcNameMap;
    outFile.close();
    return std::string(fileName);
}

////TODO: delete debug code
//static std::vector<int> HistoryOp; //0: alloc 1: free
//static std::vector<DWORD> HistoryAddress;
//static const int BIG_SIZE = 20000;
//
//static struct _A{
//    _A() {
//        HistoryOp.reserve(BIG_SIZE);
//        HistoryAddress.reserve(BIG_SIZE);
//    }
//} _a;
//
//static void FlushHistory()
//{
//    FILE* fp = fopen("history.txt", "a+");
//    for (int i = 0; i < HistoryOp.size(); ++i)
//    {
//        fprintf(fp, "%d:%d\n", HistoryOp[i], HistoryAddress[i]);
//    }
//    HistoryOp.clear();
//    HistoryAddress.clear();
//    fclose(fp);
//}

enum BadFreeType
{
    FreeNotAlloc = 1, //not allocate but free, maybe alloc not register
    DoubleFree = 2, //maybe stats error?
};

void QSMemoryProfiler::ProcessPkg(void* data)
{
    const QSMemoryPkg pkg = *(const QSMemoryPkg*)data;
    switch (pkg.OpType)
    {
    case E_MOP_Register:
        {
            mGameProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pkg.RegisterInfo.handle);
        }
        break;
    case E_MOP_Alloc:
        {
            std::lock_guard<std::mutex> memStateLock(mMemStateMutex);
            auto& db = mMemState.AllocDB;
            auto& info = pkg.AllocInfo;
            auto iter = db.find(info.BaseAddress);
            
 /*           HistoryAddress.push_back(info.BaseAddress);
            HistoryOp.push_back(0);
            if (HistoryOp.size() >= BIG_SIZE)
            {
                FlushHistory();
            }
*/
            if (iter == db.end())
            {
                db[info.BaseAddress] = info;
                mMemState.TotalAllocSize += info.Count;
                mMemState.NumAllocs += 1;
            }
            else  //maybe realloc
            {
                mMemState.TotalAllocSize -= iter->second.Count;
                db[info.BaseAddress] = info;
                mMemState.TotalAllocSize += info.Count;
            }
    
            if (mMemState.TotalAllocSize > mMemState.PeakAllocSize)
            {
                mMemState.PeakAllocSize = mMemState.TotalAllocSize;
            }
        }
        break;
    
    case E_MOP_Free:
        {
            std::lock_guard<std::mutex> memStateLock(mMemStateMutex);
            auto& db = mMemState.AllocDB;
            auto& info = pkg.AllocInfo;
            auto iter = db.find(info.BaseAddress);

            //HistoryAddress.push_back(info.BaseAddress);
            //HistoryOp.push_back(1);
            //if (HistoryOp.size() >= BIG_SIZE)
            //{
            //    FlushHistory();
            //}

            if (iter != db.end())
            {
                auto& allocInfo = iter->second;
                //assert(allocInfo.Count == info.Count);
                db.erase(info.BaseAddress);
                mMemState.TotalAllocSize -= allocInfo.Count;
                mMemState.NumAllocs -= 1;       //should?
            }
            else
            {
                //something currently out of control
                printf("Free 0x%p out of our control, And never freed before\n", info.BaseAddress);

                std::lock_guard<std::mutex> LockBadFree(mBadFreeMutex);
                int TargetIndex = (mBadFreeIndex++) % BAD_FREE_RECORD_MAX;
                
                memcpy(&mBadFrees[TargetIndex][0], info.BackTrace, sizeof info.BackTrace);
                mBadFreeSize[TargetIndex] = info.Count;
            }
        }
        break;

    case E_MOP_SaveBin:
        {
            //std::lock_guard<std::mutex> memStateLock(mMemStateMutex);
            //TODO
            //SaveBinaryAllocData();
        }
        break;
    
    default:
        break;
    }
}

std::string QSMemoryProfiler::GetFunctionName(DWORD address)
{
    InitStackWalking();
    std::string ret;

    char SymBuffer[sizeof(IMAGEHLP_SYMBOL64) + 512];
    PIMAGEHLP_SYMBOL64 Symbol;
    DWORD SymbolDisplacement = 0;
    DWORD64 SymbolDisplacement64 = 0;
    
    Symbol = (PIMAGEHLP_SYMBOL64)SymBuffer;
    Symbol->SizeOfStruct = sizeof(SymBuffer);
    Symbol->MaxNameLength = 512;

    //Symbol name
    if (SymGetSymFromAddr64(mGameProcess, (DWORD64)address, &SymbolDisplacement64, Symbol))
    {
        char FunctionName[1024] = { 0 };
        int offset = 0;
        while (Symbol->Name[offset] < 32 || Symbol->Name[offset] > 127)
        {
            offset++;
        }
        sprintf_s(FunctionName, "%s() ", (const char*)(Symbol->Name + offset));
        ret += FunctionName;
    }
    //File name
    {
        IMAGEHLP_LINE64 ImageHelpLine;
        char FileNameLine[1024] = { 0 };
        ImageHelpLine.SizeOfStruct = sizeof ImageHelpLine;
        if (SymGetLineFromAddr64(mGameProcess, (DWORD64)address, &SymbolDisplacement, &ImageHelpLine))
        {
            sprintf_s(FileNameLine, "0x%-8x + %d bytes [File=%s:%d] ", address, SymbolDisplacement, ImageHelpLine.FileName, ImageHelpLine.LineNumber);
        }
        else
        {
            sprintf_s(FileNameLine, "Address = 0x%-8x (filename not found) ", address);
        }
        ret += FileNameLine;
    }
    //Module
    {
        IMAGEHLP_MODULE64 ImageHelpModule;
        char ModuleName[1024] = { 0 };
        ImageHelpModule.SizeOfStruct = sizeof ImageHelpModule;
        if (SymGetModuleInfo64(mGameProcess, (DWORD64)address, &ImageHelpModule))
        {
            sprintf_s(ModuleName, "[in %s]", ImageHelpModule.ModuleName);
        }
        ret += ModuleName;
    }
    return ret;
}


void QSMemoryProfiler::SaveBadFrees()
{
    std::lock_guard<std::mutex> LockBadFree(mBadFreeMutex);

    std::ofstream fout;
    fout.open("badfrees.txt", std::ios::out);
    for (int i = 0; i < BAD_FREE_RECORD_MAX; ++i)
    {
        if (mBadFrees[i][0] == 0)
        {
            continue;
        }

        fout << "-------------------------------------------" << "size: " << mBadFreeSize[i] << "\n";
        for (int j = 0; j < MAX_CALLSTACK_DEPTH; ++j)
        {
            if (mBadFrees[i][j] == 0)
            {
                break;
            }
            fout << GetFunctionName(mBadFrees[i][j]) << "\n";
        }
    }
    fout.close();
}

/* symbol utilities */
typedef BOOL(WINAPI *TFEnumProcessModules)(HANDLE hProcess, HMODULE *lphModule, DWORD cb, LPDWORD lpcbNeeded);
typedef DWORD(WINAPI *TFGetModuleBaseName)(HANDLE hProcess, HMODULE hModule, LPSTR lpBaseName, DWORD nSize);
typedef DWORD(WINAPI *TFGetModuleFileNameEx)(HANDLE hProcess, HMODULE hModule, LPSTR lpFilename, DWORD nSize);
typedef BOOL(WINAPI *TFGetModuleInformation)(HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb);

static TFEnumProcessModules		FEnumProcessModules;
static TFGetModuleBaseName		FGetModuleBaseName;
static TFGetModuleFileNameEx	FGetModuleFileNameEx;
static TFGetModuleInformation	FGetModuleInformation;

void QSMemoryProfiler::InitStackWalking()
{
    if (!mGStackWalkingInitialized)
    {
        HMODULE DllHandle = LoadLibraryA("PSAPI.DLL");

        FEnumProcessModules = (TFEnumProcessModules)GetProcAddress(DllHandle, "EnumProcessModules");
        FGetModuleBaseName = (TFGetModuleBaseName)GetProcAddress(DllHandle, "GetModuleBaseNameA");
        FGetModuleFileNameEx = (TFGetModuleFileNameEx)GetProcAddress(DllHandle, "GetModuleFileNameExA");
        FGetModuleInformation = (TFGetModuleInformation)GetProcAddress(DllHandle, "GetModuleInformation");

        DWORD SymOpts = SymGetOptions();
        SymOpts |= SYMOPT_LOAD_LINES;   //source code line
        SymOpts |= SYMOPT_DEBUG;        //symbol search debug information
        SymOpts |= SYMOPT_UNDNAME;      //undecorate names
        SymOpts |= SYMOPT_FAIL_CRITICAL_ERRORS; //suppress file access error dialog
        SymOpts |= SYMOPT_DEFERRED_LOADS;
        SymOpts |= SYMOPT_ALLOW_ABSOLUTE_SYMBOLS;
        SymOpts |= SYMOPT_EXACT_SYMBOLS; //perform strict evaluation of symbol files
        SymOpts |= SYMOPT_CASE_INSENSITIVE;
        SymSetOptions(SymOpts);

        char  userPath[1024];
        _snprintf_s(userPath, 1024, "C:\\websymbols;C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Extras\\Symbols");
        SymInitialize(mGameProcess, userPath, TRUE);
        LoadProcessModules();
        mGStackWalkingInitialized = true;
    }
}

void QSMemoryProfiler::LoadProcessModules()
{
    const int MAX_MOD_HANDLES = 1024;
    HMODULE ModuleHandleArray[MAX_MOD_HANDLES];
    HMODULE* ModuleHandlePointer = ModuleHandleArray;
    MODULEINFO ModuleInfo;

    DWORD BytesRequired;
    BOOL EnumProcessModuleSuccess = FEnumProcessModules(mGameProcess, ModuleHandleArray, sizeof ModuleHandleArray, &BytesRequired);
    if (!EnumProcessModuleSuccess)
    {
        return;
    }

    bool NeedToFreeHandlePointer = false;
    if (BytesRequired > sizeof ModuleHandleArray)
    {
        NeedToFreeHandlePointer = true;
        ModuleHandlePointer = (HMODULE*)malloc(BytesRequired);
        FEnumProcessModules(mGameProcess, ModuleHandlePointer, BytesRequired, &BytesRequired);
    }

    int ModuleCount = BytesRequired / sizeof(HMODULE);
    for (int i = 0;i < ModuleCount; ++i)
    {
        char ModuleName[1024];
        char ImageName[1024];
        FGetModuleInformation(mGameProcess, ModuleHandlePointer[i], &ModuleInfo, sizeof ModuleInfo);
        FGetModuleFileNameEx(mGameProcess, ModuleHandlePointer[i], ImageName, 1024);
        FGetModuleBaseName(mGameProcess, ModuleHandlePointer[i], ModuleName, 1024);

        char SearchPath[1024];
        char* FileName = nullptr;
        GetFullPathNameA(ImageName, 1024, SearchPath, &FileName);
        *FileName = 0;

        strcat_s(SearchPath, ";C:\\QSSymbols;C:\\websymbols;C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Extras\\Symbols");
        SymSetSearchPath(mGameProcess, SearchPath);

        SymLoadModule64(mGameProcess, ModuleHandleArray[i], ImageName, ModuleName, (DWORD64)ModuleInfo.lpBaseOfDll, (DWORD64)ModuleInfo.SizeOfImage);
    }

    if (NeedToFreeHandlePointer)
    {
        free(ModuleHandlePointer);
    }
}

QSMemoryView::QSMemoryView(QSMemoryProfiler& profiler): mProfilerRef(profiler)
{
    memset(mTotalMemRecord, 0, sizeof mTotalMemRecord);
    mRecordInsertIdx = 0;
    mVisible = false;
    mShowDumpView = false;
    mShowDiffView = false;
    mLastCheckT = 0.f;
    mCurTotalAlloc = mCurPeakAlloc = 0.f;
    mCurAllocNum = 0;
}

void QSMemoryView::CheckStats(float time)
{
    if (time >= mLastCheckT + 0.5f)
    {
        mLastCheckT = time;

        mProfilerRef.GetStats(mCurTotalAlloc, mCurPeakAlloc, mCurAllocNum);
        if (mRecordInsertIdx == MemRecordNum)
        {
            mRecordInsertIdx = 0;
        }
        mTotalMemRecord[mRecordInsertIdx++] = mCurTotalAlloc;
    }
}

struct TreeNode
{
    int64_t TotalSize = 0;
    int64_t CallTimes = 0;
    DWORD address = 0;
    std::string FuncName;

    std::map<DWORD, TreeNode*> ChildMap;
    std::vector<TreeNode*> Children;
    bool sorted = false;

    TreeNode* FindAndRegisterChild(DWORD address, QSMemoryProfiler& profiler);
    void ImDisplay(QSMemoryProfiler& ProfilerRef);
    static bool ShoudSkip(const std::string& FuncName)
    {
        if (FuncName.find("not f") != std::string::npos)
            return true;
        //if (FuncName.find("HookHeapAlloc") != std::string::npos)
        //    return true;
        //if (FuncName.find("QSMallocWindows") != std::string::npos)
        //    return true;
        return false;
    }
    static void MergeChild(TreeNode*parent, TreeNode*child)
    {
        auto iter = parent->ChildMap.find(child->address);
        if (iter == parent->ChildMap.end())
        {
            parent->ChildMap[child->address] = child;
        }
        else
        {
            iter->second->TotalSize += child->TotalSize;
            iter->second->CallTimes += child->CallTimes;
            for (auto c: child->ChildMap)
            {
                MergeChild(iter->second, c.second);
            }
            delete child;
        }
    }
};

void TreeNode::ImDisplay(QSMemoryProfiler& ProfilerRef)
{
    const bool ISROOT = address == 0;
    bool ShowChild = false;
    if (ISROOT)
    {
        ShowChild = ImGui::TreeNode("Root", "[Total alloc size:] %lldK", TotalSize / 1024);
    }
    else
    {
        if (FuncName.empty())
        {
            FuncName = ProfilerRef.GetFunctionName(address);
        }
        ShowChild = ImGui::TreeNode(this, "[size] %lldK [call time] %lld %s", TotalSize / 1024, CallTimes, FuncName.c_str());
    }
    if (ShowChild)
    {
        if (!sorted) // first time to expand
        {
            while (true)
            {
                TreeNode* ChildToOmit = nullptr;
                for (auto iter = ChildMap.begin(); iter != ChildMap.end(); ++iter)
                {
                    TreeNode* c = iter->second;
                    if (c->FuncName.empty())
                    {
                        c->FuncName = ProfilerRef.GetFunctionName(c->address);
                    }

                    if (ShoudSkip(c->FuncName))
                    {
                        ChildToOmit = c;
                        ChildMap.erase(iter);
                        break;
                    }
                }
                if (!ChildToOmit)
                {
                    break;
                }
                else
                {
                    for (auto c: ChildToOmit->ChildMap)
                    {
                        MergeChild(this, c.second);
                    }
                    delete ChildToOmit;
                }
            }

            Children.reserve(ChildMap.size());
            for (auto c: ChildMap)
            {
                Children.push_back(c.second);
            }
            ChildMap.clear();
            std::sort(Children.begin(), Children.end(), [](const TreeNode*a, const TreeNode* b) { return a->TotalSize > b->TotalSize; });
            sorted = true;
        }
        
        for (auto c: Children)
        {
            c->ImDisplay(ProfilerRef);
        }
 
        ImGui::TreePop();
    }
};

TreeNode* TreeNode::FindAndRegisterChild(DWORD address, QSMemoryProfiler& profiler)
{
    auto iter = ChildMap.find(address);
    if (iter != ChildMap.end())
    {
        return iter->second;
    }
    else
    {
        //std::string FuncName = profiler.GetFunctionName(address);
        //if (ShoudSkip(FuncName))
        //{
        //    return this;
        //}
        //else
        {
            TreeNode* newNode = new TreeNode;
            newNode->address = address;
            newNode->FuncName = FuncName;
            ChildMap[address] = newNode;
            return newNode;
        }
    }
}

void QSMemoryView::ImDraw(float time)
{
    if (mVisible)
    {
        ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("Memory Profiler", &mVisible))
        {
            ImGui::End();
            return;
        }

        if (ImGui::TreeNodeEx("Memory Stats", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PlotLines("##TotalMem", mTotalMemRecord, MemRecordNum, mRecordInsertIdx, NULL, 0.f, FLT_MAX, ImVec2(0, 80));

            ImGui::Text("TotalMem: "); ImGui::SameLine(150); ImGui::Text("%.3f MB", mCurTotalAlloc);
            ImGui::Text("PeakMem:  "); ImGui::SameLine(150); ImGui::Text("%.3f MB", mCurPeakAlloc);
            ImGui::Text("AllocNum: "); ImGui::SameLine(150); ImGui::Text("%lld", mCurAllocNum);
            ImGui::TreePop();
        }
        ImGui::Spacing();
        if (ImGui::TreeNodeEx("Allocation Dumps", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::Button("snapshot"))
            {
                DumpRecord record;
                record.Time = time;
                record.DumpName = mProfilerRef.SaveBinaryAllocData(record.Total, record.num);
                mDumpRecords.push_back(record);
            }
            ImGui::SameLine(); 
            if (ImGui::Button("view"))
            {
                int id = -1;
                for (int i = 0;i < mDumpRecords.size(); ++i)
                {
                    if (mDumpRecords[i].selected)
                    {
                        id = i;
                        break;
                    }
                }
                if (id >= 0)
                {
                    DumpView(mDumpRecords[id].DumpName);
                }
            }
            ImGui::SameLine(); 
            if (ImGui::Button("Delete"))
            {
                for (int i = 0; i < mDumpRecords.size(); ++i)
                {
                    if (mDumpRecords[i].selected)
                    {
                        fs::remove(mDumpRecords[i].DumpName);
                        mDumpRecords.erase(mDumpRecords.begin() + i);
                        break;
                    }
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("IllegalFree"))
            {
                mProfilerRef.SaveBadFrees();
            }
            
            static ImGui::ColumnHeader headers[] = {
                { "ID",  -1, -1, false},
                { "TIME", 80, -1, false},
                { "TOTAL", 200, -1, false},
                { "NUM", 200, -1, true}
            };

            ImGui::ColumnHeaders("MemDumpHeader", headers, IM_ARRAYSIZE(headers), true);
            ImGui::BeginColumnHeadersSync("MemDumpContent", headers, IM_ARRAYSIZE(headers), false);

            std::vector<int> selected;
            for (int i = 0;i < mDumpRecords.size(); ++i)
            {
                char label[10] = { 0 };
                sprintf_s(label, "#%d", i + 1);
                if (ImGui::Selectable(label, mDumpRecords[i].selected, ImGuiSelectableFlags_SpanAllColumns))
                {
                    mDumpRecords[i].selected = !mDumpRecords[i].selected;
                }
                ImGui::NextColumn();
                ImGui::Text("%.2fs", mDumpRecords[i].Time);
                ImGui::NextColumn();
                ImGui::Text("%.2fKB", mDumpRecords[i].Total);
                ImGui::NextColumn();
                ImGui::Text("%lld", mDumpRecords[i].num);
                ImGui::NextColumn();

                if (mDumpRecords[i].selected)
                {
                    selected.push_back(i);
                }
            }
            ImGui::EndColumnHeadersSync(headers, IM_ARRAYSIZE(headers));

            if (selected.size() == 2 && ImGui::Button("Diff"))
            {
                DiffDump(mDumpRecords[selected[0]].DumpName, mDumpRecords[selected[1]].DumpName);
            }
            ImGui::TreePop();
        }
        ImGui::End();

        if (mShowDumpView)
        {
            if (ImGui::Begin("Dump View", &mShowDumpView, ImGuiWindowFlags_HorizontalScrollbar))
            {
                mTreeRoot->ImDisplay(mProfilerRef);
            }
            ImGui::End();
        }

        if (mShowDiffView)
        {
            if (ImGui::Begin("Diff View", &mShowDiffView, ImGuiWindowFlags_HorizontalScrollbar))
            {
                mDiffRoot->ImDisplay(mProfilerRef);
            }
            ImGui::End();
        }
    }
}

void QSMemoryView::SetVisible(bool flag)
{
    mVisible = flag;
    if (!mVisible)
    {
        mShowDumpView = false;
        mShowDiffView = false;
    }
}

void QSMemoryView::FreeTreeNode(TreeNode* node)
{
    if (node)
    {
        for (auto ptr: node->Children)
        {
            FreeTreeNode(ptr);
        }
        delete node;
    }
}

void QSMemoryView::DiffDump(const std::string& File1, const std::string& File2)
{
    std::ifstream inFile1, inFile2;
    inFile1.open(File1, std::ios::binary | std::ios::in);
    inFile2.open(File2, std::ios::binary | std::ios::in);
    if (inFile1.good() && inFile2.good())
    {
        std::map<DWORD, QSGroupedAllocInfo> GroupedAllocations;
        //new first
        Serialize(inFile2, GroupedAllocations);
        inFile2.close();

        mShowDiffView = true;

        FreeTreeNode(mDiffRoot);
        mDiffRoot = new TreeNode;
        TreeNode* CurrentNode = nullptr;
        for (auto it = GroupedAllocations.begin(); it != GroupedAllocations.end(); ++it)
        {
            int BottomIdx = MAX_CALLSTACK_DEPTH - 1;
            for (; it->second.BackTrace[BottomIdx] == 0; --BottomIdx);

            CurrentNode = mDiffRoot;
            CurrentNode->TotalSize += it->second.TotalSize;
            CurrentNode->CallTimes += it->second.Count;

            if (mTopDown)
            {
                for (int i = BottomIdx; i >= 0; i--)
                {
                    TreeNode* Tmp = CurrentNode->FindAndRegisterChild(it->second.BackTrace[i], mProfilerRef);
                    if (Tmp != CurrentNode)
                    {
                        Tmp->TotalSize += it->second.TotalSize;
                        Tmp->CallTimes += it->second.Count;
                        CurrentNode = Tmp;
                    }
                }
            }
            else
            {
                for (int i = 0; i <= BottomIdx; ++i)
                {
                    TreeNode* Tmp = CurrentNode->FindAndRegisterChild(it->second.BackTrace[i], mProfilerRef);
                    if (Tmp != CurrentNode)
                    {
                        Tmp->TotalSize += it->second.TotalSize;
                        Tmp->CallTimes += it->second.Count;
                        CurrentNode = Tmp;
                    }
                }
            }
        }

        GroupedAllocations.clear();
        Serialize(inFile1, GroupedAllocations);
        inFile1.close();

        CurrentNode = nullptr;
        for (auto it = GroupedAllocations.begin(); it != GroupedAllocations.end(); ++it)
        {
            int BottomIdx = MAX_CALLSTACK_DEPTH - 1;
            for (; it->second.BackTrace[BottomIdx] == 0; --BottomIdx);

            CurrentNode = mDiffRoot;
            CurrentNode->TotalSize -= it->second.TotalSize;
            CurrentNode->CallTimes -= it->second.Count;

            if (mTopDown)
            {
                for (int i = BottomIdx; i >= 0; i--)
                {
                    TreeNode* Tmp = CurrentNode->FindAndRegisterChild(it->second.BackTrace[i], mProfilerRef);
                    if (Tmp != CurrentNode)
                    {
                        Tmp->TotalSize -= it->second.TotalSize;
                        Tmp->CallTimes -= it->second.Count;
                        CurrentNode = Tmp;
                    }
                }
            }
            else
            {
                for (int i = 0; i <= BottomIdx; ++i)
                {
                    TreeNode* Tmp = CurrentNode->FindAndRegisterChild(it->second.BackTrace[i], mProfilerRef);
                    if (Tmp != CurrentNode)
                    {
                        Tmp->TotalSize -= it->second.TotalSize;
                        Tmp->CallTimes -= it->second.Count;
                        CurrentNode = Tmp;
                    }
                }
            }
        }
    }
    else
    {
        inFile1.close();
        inFile2.close();
    }
}

void QSMemoryView::DumpView(const std::string& File)
{
    //mShowDumpView = true;

    //if (File != mCurDump || mTopDown != TopDown)
    {
        std::ifstream inFile;
        inFile.open(File, std::ios::binary | std::ios::in);
        if (inFile.good())
        {
            //mCurDump = File;
            //mTopDown = TopDown;

            FreeTreeNode(mTreeRoot);

            std::map<DWORD, QSGroupedAllocInfo> GroupedAllocations;
            Serialize(inFile, GroupedAllocations);
            inFile.close();

            mShowDumpView = true;

            mTreeRoot = new TreeNode;
            TreeNode* CurrentNode = nullptr;

            for (auto it = GroupedAllocations.begin(); it != GroupedAllocations.end(); ++it)
            {
                int BottomIdx = MAX_CALLSTACK_DEPTH - 1;
                for (; it->second.BackTrace[BottomIdx] == 0; --BottomIdx);

                CurrentNode = mTreeRoot;
                CurrentNode->TotalSize += it->second.TotalSize;
                CurrentNode->CallTimes += it->second.Count;

                if (mTopDown)
                {
                    for (int i = BottomIdx; i >= 0; i --)
                    {
                        TreeNode* Tmp = CurrentNode->FindAndRegisterChild(it->second.BackTrace[i], mProfilerRef);
                        if (Tmp != CurrentNode)
                        {
                            Tmp->TotalSize += it->second.TotalSize;
                            Tmp->CallTimes += it->second.Count;
                            CurrentNode = Tmp;
                        }
                    }
                }
                else
                {
                    for (int i = 0;i <= BottomIdx; ++i)
                    {
                        TreeNode* Tmp = CurrentNode->FindAndRegisterChild(it->second.BackTrace[i], mProfilerRef);
                        if (Tmp != CurrentNode)
                        {
                            Tmp->TotalSize += it->second.TotalSize;
                            Tmp->CallTimes += it->second.Count;
                            CurrentNode = Tmp;
                        }
                    }
                }
            }
        }
        else
        {
            inFile.close();
        }
    }
}