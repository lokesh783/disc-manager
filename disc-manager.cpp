#include <iostream>
#include <string>
#include "diskManager.h"
#include "windows.h"
using namespace std;

void handleDisplayDiskFreeSpace(DiskSpaceManager& manager) {
    vector <wstring> drives = manager.getAllDrives();
    int i = 1, driveNum;
    for (const wstring& drive : drives) {
        wcout << "To select drive " << drive << " press " << i++ << endl;
    }
    cin >> driveNum;

    if (driveNum < 0 || driveNum >= drives.size()) {
        cout << "wrong input" << endl;
        return;
    }
    manager.getDriveSpaceInfo(drives[driveNum-1], 0, 0, 1);
}

void handleDetectDuplicateFiles(DiskSpaceManager& manager) {
    string dirPath; cin >> dirPath;
    manager.findDuplicateFiles(dirPath);
}

void handleDisplayDiskUsedSpace(DiskSpaceManager& manager) {
    vector <wstring> drives = manager.getAllDrives();
    int i = 1, driveNum;
    for (const wstring& drive : drives) {
        wcout << "To select drive " << drive << " press " << i++ << endl;
    }
    cin >> driveNum;
    if (driveNum < 0 || driveNum >= drives.size()) {
        cout << "wrong input" << endl;
        return;
    }
    manager.getDriveSpaceInfo(drives[driveNum - 1], 0, 1, 0);
}

void handleIdentifyLargeFiles(DiskSpaceManager& manager) {
    cout << "please enter directory path you wish to Identify large files in" << endl;
    string dirPath; cin >> dirPath;
    cout << "size required" << endl;
    long long threshold; cin >> threshold;
    manager.identifyLargeFiles(dirPath, threshold);
}

void handleScanSpecificTypeFiles(DiskSpaceManager& manager) {
    cout << "Input directory and file type you want to scan" << endl;
    string dirPath, ext = ""; cin >> dirPath >> ext;
    manager.scanSpecificTypeFiles(dirPath, ext);
}
void handleDeleteFile(DiskSpaceManager& manager) {
    cout << "Input file path to be delete" << endl;
    string dirPath; cin >> dirPath;
    manager.deleteFile(dirPath);
}

void handleDeleteFolder(DiskSpaceManager& manager) {
    cout << "Input folder path to be delete" << endl;
    string dirPath; cin >> dirPath;
    manager.deleteFolder(dirPath);
}

void handleDeleteFilesByType(DiskSpaceManager& manager) {
    cout << "Input directory and file typeto be deleted" << endl;
    string dirPath, ext = ""; cin >> dirPath >> ext;
    manager.deleteFilesByType(dirPath, ext);
}

void handleAnalyzeSpaceBreakdown(DiskSpaceManager& manager) {
    cout << "Input directory to be analyzed" << endl;
    string dirPath; cin >> dirPath ;
    manager.analyzeSpaceBreakdown(dirPath); 
}

void handleAnalyzeDriveSpaceBreakdown(DiskSpaceManager& manager) {
    manager.analyzeDriveSpaceBreakdown();
}

void handleCalls(DiskSpaceManager& manager, bool & quit) {
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
        // Call getRequiredSpace() function here
            handleDisplayDiskFreeSpace(manager);
            break;
        case 2:
        //    // Call displayDiskFreeSpace() function here
            handleDisplayDiskUsedSpace(manager);
            break;
        case 3:
            // Call findDuplicateFiles() function here
            handleDetectDuplicateFiles(manager);
            break;
        case 4:
            // Call identifyLargeFiles() function here
            handleIdentifyLargeFiles(manager);
            break;
        case 5:
            // Call scanSpecificTypeFiles() function here
            handleScanSpecificTypeFiles(manager);
            break;
        case 6:
            // Call deleteFile() function here
            handleDeleteFile(manager);
            break;
        case 7:
            // Call deleteFolder() function here
            handleDeleteFolder(manager);
            break;
        case 8:
            // Call deleteFilesByType() function here
            handleDeleteFilesByType(manager);
            break;
        case 9:
            // Call analyzeSpaceBreakdown() function here
            handleAnalyzeSpaceBreakdown(manager);
            break;
        case 10:
            // Call analyzeDriveSpaceBreakdown() function here
            handleAnalyzeDriveSpaceBreakdown(manager);
            break;
        case -1:
        // Quit the program
            std::cout << "Ending process..." << std::endl;
            quit = 1;
            return;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        break;
    }
}

int main() {
    string rootPath = "C:\\";
    DiskSpaceManager manager(rootPath);

    cout << "System Started" << endl;

    while (true) {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Available Functions:" << std::endl;
        std::cout << "1. Display Disk Free Space" << std::endl;
        std::cout << "2. Display Utilized Space" << std::endl;
        std::cout << "3. Find Duplicate Files" << std::endl;
        std::cout << "4. Identify Large Files" << std::endl;
        std::cout << "5. Scan Specific Type Files" << std::endl;
        std::cout << "6. Delete File" << std::endl;
        std::cout << "7. Delete Folder" << std::endl;
        std::cout << "8. Delete Files By Type" << std::endl;
        std::cout << "9. Analyze Space Breakdown" << std::endl;
        std::cout << "10. Analyze Drive Space Breakdown" << std::endl;
        std::cout << "-1. Quit" << std::endl;

        bool quit = 0;
        handleCalls(manager, quit);
        if (quit) break;
    }
    return 0;
}