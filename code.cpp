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
using namespace std;

#ifndef MD5_DIGEST_LENGTH
#define MD5_DIGEST_LENGTH 16
#endif

DiskSpaceManager::DiskSpaceManager(const std::string& rootPath) : rootPath_(rootPath) {
    rootPath_ = rootPath;
    retrivedDiskSpace = 0;
    getRequiredSpace();
}

void DiskSpaceManager::getRequiredSpace() {
    LPCSTR  lpRootPathName = rootPath_.c_str();
    DWORD sectorsPerCluster;
    DWORD bytesPerSector;
    DWORD freeClusters;
    DWORD totalClusters;

    if (GetDiskFreeSpaceA(lpRootPathName, &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters)) {
        ULONGLONG totalSpace = (ULONGLONG)totalClusters * sectorsPerCluster * bytesPerSector;
        ULONGLONG freeSpace = (ULONGLONG)freeClusters * sectorsPerCluster * bytesPerSector;

        totalDiskSpace = totalSpace;
        utilizedDiskSpace = totalSpace - freeSpace;
        retrivedDiskSpace = 1;
    }
    else {
        retrivedDiskSpace = 0;
    }
}

void DiskSpaceManager::displayUtilizedSpace() {
    if (retrivedDiskSpace) cout << "Utilized disk space: " << utilizedDiskSpace / (static_cast<unsigned long long>(1024) * 1024) << " MB\n";
    else cout << "Failed to retrieve disk space information.\n";
}

void DiskSpaceManager::displayDiskFreeSpace() {
    if (retrivedDiskSpace) cout << "Free disk space: " << (totalDiskSpace - utilizedDiskSpace) / (static_cast<unsigned long long>(1024) * 1024) << " MB\n";
    else cout << "Failed to retrieve disk space information.\n";
}
string calculateMD5Hash(const string& filePath) {

    cout << "Entered Inside str\n";
    std::ifstream file(filePath, ios::binary);
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

    if (!file.eof()) {
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

void DiskSpaceManager::findDuplicateFiles(const string& directoryPath) {
    cout << directoryPath << endl;
    //const string directoryPath = rootPath_;
    unordered_map<string, vector<string>> fileMap;

    for (const auto& entry : filesystem::directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();
            std::string md5Hash = calculateMD5Hash(filePath);

            if (!md5Hash.empty()) {
                fileMap[md5Hash].push_back(filePath);
            }
        }
    }
    cout << "Entered 2\n";

    // Display duplicate files
    for (const auto& entry : fileMap) {
        if (entry.second.size() > 1) {
            cout << "Duplicate Files (MD5 Hash: " << entry.first << "):" << std::endl;
            for (const std::string& filePath : entry.second) {
                cout << "  " << filePath << std::endl;
            }
            cout << endl;
        }
    }
}

void DiskSpaceManager::identifyLargeFiles(const string& directoryPath, long long threshold = 100) {
    long long thresholdSizeInBytes = threshold * 1024 * 1024;
    for (const auto& entry : filesystem::recursive_directory_iterator(directoryPath)) {
        if (entry.is_regular_file()) {
            try {
                long long fileSize = filesystem::file_size(entry.path());
                if (fileSize > thresholdSizeInBytes) {
                    std::cout << "Large file: " << entry.path() << " (" << fileSize << " bytes)" << endl;
                }
            }
            catch (const filesystem::filesystem_error& ex) {
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
            catch (const filesystem::filesystem_error& error) {      // when exception of this type is thrown
                std::cerr << "Error: " << error.what() << endl;      // what specifiest the error
            }
        }
    }
}