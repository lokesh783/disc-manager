#pragma once
#include <string>
#include <windows.h>
using namespace std;

class DiskSpaceManager {
public:
    DiskSpaceManager(const string& rootPath);
    void getRequiredSpace();
    void displayDiskFreeSpace();
    void displayUtilizedSpace();
    void findDuplicateFiles(const string& directoryPath);
    void identifyLargeFiles(const string& directoryPath, long long threshold);
    void scanSpecificTypeFiles(const string& directoryPath, const string& ext);

private:
    string rootPath_;
    ULONGLONG totalDiskSpace;
    ULONGLONG utilizedDiskSpace;
    bool retrivedDiskSpace;
};
