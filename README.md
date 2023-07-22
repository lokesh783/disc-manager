# Disk Space Manager

Disk Space Manager is a C++ program that provides comprehensive insights into disk space utilization and facilitates efficient management of disk space. The program uses Windows library functions and is developed to work with both Release and Debug builds. It is designed to be compatible with C++17 standard.

## Features

- Display the amount of free space available on the disk.
- Present the amount of space utilized on the disk.
- Provide a breakdown of space utilization based on specific paths and file types (e.g., video, images).
- Detect duplicate files and display their paths.
- Identify large files based on a user-defined threshold size.
- Scan and display files of specific types in a directory.
- Enable faster and efficient deletion of files and folders.
- Allow users to delete files of specific types.

## Getting Started

To use the Disk Space Manager, follow these steps:

1. Clone the repository or download the source code to your local machine.

2. Open the project in Microsoft Visual Studio or any other C++ development environment that supports C++17.

3. Build the project.

4. Run the executable, and the program will provide a menu of available functions to choose from.

## Usage

Upon running the program, you will be presented with a menu of available functions:

1. **Display Disk Free Space**: Displays the amount of free space available on the disk.

2. **Display Utilized Space**: Displays the amount of space utilized on the disk.

3. **Find Duplicate Files**: Detects duplicate files in a specified directory.

4. **Identify Large Files**: Identifies and displays large files in a directory based on a user-defined threshold size.

5. **Scan Specific Type Files**: Scans and displays files of a specific file type in a directory.

6. **Delete File**: Deletes a specified file.

7. **Delete Folder**: Deletes a specified folder and its contents.

8. **Delete Files By Type**: Deletes files of a specific file type in a directory.

9. **Analyze Space Breakdown**: Provides a breakdown of space utilization based on types of files (video, images, etc.) in a specified directory.

10. **Analyze Drive Space Breakdown**: Provides a breakdown of space utilization for all available drives on the system.

Enter the number corresponding to the desired function to perform the respective operation.

## Requirements

- Microsoft Visual Studio or any other C++ development environment that supports C++17.
- Windows operating system.

## Attributions

# Understanding
It was my first reference to know a bit about Local File Systems
https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/aa364407(v=vs.85)

# API TO FIND THE NUMBER OF DRIVES AVAILABLE
https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getlogicaldrives

# API TO GET FREE SPACE
https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getdiskfreespacea

# DETECT DUPLICATE FILES
https://www.geeksforgeeks.org/what-is-the-md5-algorithm/
https://www.openssl.org/docs/man1.1.1/man3/MD5.html

# OPENSSL DIDN'T WORKED, SO I NEED TO SHIFT TO WINDOWS MD5
https://learn.microsoft.com/en-us/windows/win32/seccrypto/example-c-program--creating-an-md-5-hash-from-file-content

# DIRECTORY TRAVERSALS
https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
https://stackoverflow.com/questions/67273/how-do-you-iterate-through-every-file-directory-recursively-in-standard-c

# DELETING FILES IN BULK
https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shfileoperationa
https://learn.microsoft.com/en-us/windows/win32/shell/manage

# DELETING A FOLDER EFFICIENTLY
https://en.cppreference.com/w/cpp/filesystem/remove

# GET NUMBER OF DRIVES AVAILABLE
https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getlogicaldrivestrings

---
