#include <windows.h>
#include <iostream>


using namespace std;

int main() {

    //Separate lines to open separate cmd instances.
    //Otherwise Cat would open within the original cmd instance
    system("cmd /c start Fox.exe");
    system("cmd /c start Cat.exe");

    return 0;
}