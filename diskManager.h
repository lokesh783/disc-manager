#pragma once
#include <string>
#include <windows.h>
#include <vector>
using namespace std;

class DiskSpaceManager {
public:
    DiskSpaceManager(const string& rootPath);
    void findDuplicateFiles(const string& directoryPath);
    void identifyLargeFiles(const string& directoryPath, long long threshold);
    void scanSpecificTypeFiles(const string& directoryPath, const string& ext);
    void deleteFile(const string& directoryPath);
    void deleteFolder(const string& directoryPath);
    void deleteFilesByType(const string& directoryPath, const string& ext);
    void analyzeSpaceBreakdown(const string& directoryPath);
    vector<wstring> getAllDrives();
    void getDriveSpaceInfo(const wstring& drive, int TS, int US, int FS);
    void analyzeDriveSpaceBreakdown();

private:
    string rootPath_;
    ULONGLONG totalDiskSpace;
    ULONGLONG utilizedDiskSpace;
    bool retrivedDiskSpace;
};
