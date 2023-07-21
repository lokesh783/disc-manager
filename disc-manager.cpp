#include <iostream>
#include "diskManager.h"
//#include "code.cpp"
using namespace std;

int main() {
    string rootPath = "C:\\";
    DiskSpaceManager manager(rootPath);

    manager.scanSpecificTypeFiles("D:\\Coding\\Development\\Final\\Testing", ".txt");
    cout << "Hello world\n";
    return 0;
}