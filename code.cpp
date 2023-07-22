#include <iostream>
#include "diskManager.h"
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <wincrypt.h>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

using namespace std;

#ifndef MD5_DIGEST_LENGTH
#define MD5_DIGEST_LENGTH 16
#endif

DiskSpaceManager::DiskSpaceManager(const std::string& rootPath) : rootPath_(rootPath) {
    rootPath_ = rootPath;
    retrivedDiskSpace = 0;
}

//HASH THE CONTENT
string calculateMD5Hash(const string& filePath) {

    cout << "Entered Inside str\n";
    std::ifstream file(filePath, ios::binary);         // READ THE FILE
    if (!file) {
        std::cerr << "Error: Cannot open file '" << filePath << "'. Skipping." << std::endl;
        return "";
    }

    HCRYPTPROV hProv;
    HCRYPTHASH hHash;
    if (!CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        std::cerr << "Error: CryptAcquireContext failed." << std::endl;
        return "";
    }

    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
        std::cerr << "Error: CryptCreateHash failed." << std::endl;
        CryptReleaseContext(hProv, 0);
        return "";
    }

    const int bufferSize = 1024;
    unsigned char buffer[bufferSize] = {'a'};
    DWORD bytesRead;

    while (file.read(reinterpret_cast<char*>(buffer), bufferSize)) {
        bytesRead = static_cast<DWORD>(file.gcount());
        if (!CryptHashData(hHash, buffer, bytesRead, 0)) {
            std::cerr << "Error: CryptHashData failed." << std::endl;
            CryptDestroyHash(hHash);
            CryptReleaseContext(hProv, 0);
            return "";
        }
    }

    if (!file.eof()) { // IN CASE FILE CAN'T BE READ
        std::cerr << "Error: Failed to read the entire file '" << filePath << "'." << std::endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "";
    }

    DWORD hashSize = MD5_DIGEST_LENGTH;
    unsigned char md5Result[MD5_DIGEST_LENGTH];
    if (!CryptGetHashParam(hHash, HP_HASHVAL, md5Result, &hashSize, 0)) {
        std::cerr << "Error: CryptGetHashParam failed." << std::endl;
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "";
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        ss << hex << setfill('0') << setw(2) << static_cast<unsigned int>(md5Result[i]);
    }

    return ss.str();
}


// DETECT DUPLICATE FILES 
void DiskSpaceManager::findDuplicateFiles(const string& directoryPath) {
    //const string directoryPath = rootPath_;
    cout << directoryPath << endl;
    unordered_map<string, vector<string>> fileMap;

    // TRAVERSE THE DIRECTORY
    for (const auto& entry : filesystem::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {                             // IN CASE FILE IS REGULAR(READABLE & PERMISSIBLE)
            string filePath = entry.path().string();
            string md5Hash = calculateMD5Hash(filePath);

            if (!md5Hash.empty()) {                                // PUSH THE FILE IF IT HAS HASH
                fileMap[md5Hash].push_back(filePath);
            }
        }
    }

    // Display duplicate files
    if (!fileMap.size()) {
        cout << "No duplicate files" << endl;
        return;
    }
    for (const auto& file : fileMap) {
        if (file.second.size() > 1) {                                                 // IF DUPLICACY EXISTS
            cout << "Duplicate Files (MD5 Hash: " << file.first << "):" << std::endl; // PRINT THOSE FILES PATH
            for (const std::string& filePath : file.second) {
                cout << "  " << filePath << std::endl;
            }
            cout << endl;
        }
    }
}

// TO IDENTIFY LARGE FILES
void DiskSpaceManager::identifyLargeFiles(const string& directoryPath, long long threshold = 100) { // DEFAULT THRESHOLD 100 MB
    long long thresholdSizeInBytes = threshold * 1024 * 1024;

    // AGAIN THE SAM TRAVERSAL OVE DIRECTORY
    for (const auto& entry : filesystem::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            try {
                long long fileSize = filesystem::file_size(entry.path());
                if (fileSize > thresholdSizeInBytes) {
                    std::cout << "Large file: " << entry.path() << " (" << fileSize << " bytes)" << endl;
                }
            }
            catch (const filesystem::filesystem_error& ex) { // IN CASE WE ARE NOT ABLE TO GET THE FILE SIZE
                cerr << "Error: " << ex.what() << endl;
            }
        }
    }
}


// TO SCAN THE FILES OF SPECIFIC TYPE
void DiskSpaceManager::scanSpecificTypeFiles(const string& directoryPath, const string& fileType) {
    for (const auto& file : filesystem::recursive_directory_iterator(directoryPath)) {
        if (file.is_regular_file()) {   // If it is a regular file(can be opened)
            try {
                // Check if the file's extension matches any of the specified file types
                string extension = file.path().extension().string(); // get the extension of the file
                    if (extension == fileType) {
                        cout << "Matching file: " << file.path() << endl;
                    }
            }
            catch (const filesystem::filesystem_error& error) {         // when exception of this type is thrown
                std::cerr << "Error: " << error.what() << endl;         // what specifiest the error
            }
        }
    }
}

void DiskSpaceManager::deleteFolder(const string& folderPathTobeDelete) {
    try {
        filesystem::remove_all(folderPathTobeDelete);                    // TO REMOVE THE COMPLETE FOLDER 
                                                                         // INCLUDING CHILD ELEMENTS
        cout << "Deleted folder: " << folderPathTobeDelete << endl;
    }
    catch (const filesystem::filesystem_error& err) {
        cerr << "Error: " << err.what() << endl;
    }
}

void DiskSpaceManager::deleteFile(const std::string& filePathTobeDelete) {
    try {
        filesystem::remove(filePathTobeDelete);                            // TO REMOVE PARTICULAR FILE
        std::cout << "Deleted file: " << filePathTobeDelete << std::endl;
    }
    catch (const filesystem::filesystem_error& err) {                      // IF PROCESS IS UNSUCCESSFULL
        cerr << "Error: " << err.what() << std::endl;
    }
}

void DiskSpaceManager::deleteFilesByType(const string& directoryPath, const string& fileType) {
    vector<wstring> filesTobeDeleted;

    // Traverse the directory
    for (const auto& file : filesystem::recursive_directory_iterator(directoryPath)) {
        // IF EXTENSION OF PATH IS EQUAL TO GIVEN EXTENSION(FILE TYPE)
        // AND THE FILE IS VALID, READABLE, PERMISSIBLE
        if (file.is_regular_file() && file.path().extension() == fileType) { 
            filesTobeDeleted.push_back(file.path().wstring());
        }
    }

    // IF CONTAINER IS NOT EMPTY
    if (!filesTobeDeleted.empty()) {
        wstring filesToDelete;
        // ADD NULL TERMINATORS AT THE END
        for (const auto& filePath : filesTobeDeleted) {
            filesToDelete += filePath + L'\0';
        }
        filesToDelete += L'\0'; 


        // THIS IS A LIBRARY THAT ALLOWS US TO DELETE IN BUNDLE
        // AS WELL AS IT WILL ALLOW TO MOVE FILES TO BIN
        SHFILEOPSTRUCT fileOp;
        ZeroMemory(&fileOp, sizeof(SHFILEOPSTRUCT));
        fileOp.wFunc = FO_DELETE;
        fileOp.pFrom = filesToDelete.c_str(); 
        fileOp.pTo = nullptr;
        fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;  // FOF_ALLOWUNDO ALLOWS TO MOVE TO RECYCLE BIN

        int filesLeft = SHFileOperation(&fileOp);  // 1 REFERS TO ERROR AND 0 REFERS TO SUCCESS
        if (filesLeft == 0) {
            wcout << "Deleted " << filesTobeDeleted.size() << " files." << endl;
        }
        else {
            cerr << "Error deleting files." << endl;
        }
    }
    else {
        cout << "No files of type " << fileType << " found to delete." << endl;
    }
}

void DiskSpaceManager::analyzeSpaceBreakdown(const string& directoryPath) {
    unordered_map<string, long long> spaceUtilBreakdown;                   // STORE DIFFERENT TYPE OF FILE AS KEY
    for (const auto& file: filesystem::recursive_directory_iterator(directoryPath)) { // ITERATE RECURSIVELY
        if (file.is_regular_file()) {                                      
            string fileExt = file.path().extension().string();             // EXTRACT THE EXTENSION
            transform(fileExt.begin(), fileExt.end(), fileExt.begin(), [](unsigned char c) {
                return tolower(c);                                         // EXTENSION IS LOWERED
            });

            try {
                long long fileSize = filesystem::file_size(file.path());
                spaceUtilBreakdown[fileExt] += fileSize;
            }
            catch (const filesystem::filesystem_error& err) {
                // IN CASE NOT ABLE TO READ THE FILE
            }
        }
    }
    // IF NO FILE EXIST IN THE CURRENT DIRECTORY
    if (!spaceUtilBreakdown.size()) {
        cout << "No files are there in current directory" << endl;
        return;
    }
    cout << "Space Utilization by File Types:" << endl;

    // PRINT THE TYPE AND SIZE
    for (const auto& pair : spaceUtilBreakdown) {
        std::cout << pair.first << ": " << pair.second / (static_cast<unsigned long long>(1024) * 1024) << " MB" << std::endl;
    }
}

// TO GET ALL THE DIFFERENT DRIVES AVAILABLE IN THE SYSTE,
vector<wstring> DiskSpaceManager::getAllDrives() {
    vector<wstring> drives;
    WCHAR buffer[MAX_PATH];

    // WIN 32 LIBRARY THAT ALLOWS US TO GET DIFFERENT DRIVES
    if (GetLogicalDriveStringsW(MAX_PATH, buffer)) {
        WCHAR* current = buffer;
        while (*current != L'\0') {
            drives.push_back(std::wstring(current));
            current += wcslen(current) + 1;
        }
    }
    return drives;
}

// USED TO GET INFORMATION ABOUT DRIVE
// IT WILL BE USED TO DO ALL TYPE OF 
// DRIVE OPS
void DiskSpaceManager::getDriveSpaceInfo(const wstring& drive, int TS = 1, int US = 1, int FS = 1) {
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
    if (GetDiskFreeSpaceEx(drive.c_str(), &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
        ULONGLONG totalSpace = totalNumberOfBytes.QuadPart;
        ULONGLONG freeSpace = totalNumberOfFreeBytes.QuadPart;
        ULONGLONG usedSpace = totalSpace - freeSpace;

        wcout << "Drive: " << drive << std::endl;
        if(TS) wcout << "Total Space: " << totalSpace / (static_cast<unsigned long long>(1024) * 1024) << " MB" << std::endl;
        if(FS)wcout << "Free Space: " << freeSpace / (static_cast<unsigned long long>(1024) * 1024) << " MB" << std::endl;
        if (US)wcout << "Used Space: " << usedSpace / (static_cast<unsigned long long>(1024) * 1024) << " MB" << std::endl;
        wcout << endl;
    }
    else {
        std::wcerr << "Error getting drive space information for " << drive << std::endl;
    }
}


void DiskSpaceManager::analyzeDriveSpaceBreakdown() {
    vector<wstring> drives = getAllDrives();

    for (const wstring& drive : drives) {
        getDriveSpaceInfo(drive);
    }
}

