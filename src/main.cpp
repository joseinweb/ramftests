#include <iostream>
#include "ThunderBridge.hpp"
using namespace std;

ThunderBridge thunderBridge;

void printMainMenu() {
    cout << "--------------------------" << endl;
    cout << "RDK App Managers Test Utility" << endl;
    cout << "Select an option:" << endl;
    cout << "1. Check RDK App managers status" << endl;
    cout << "2. App Manager related functions" << endl;
    cout << "3. Package Manager related functions" << endl;
    cout << "0 . Exit" << endl;
    cout << "--------------------------" << endl;
}
int processUserCommands() {
    int choice;
    while (true) {
        printMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Checking RDK App managers status..." << endl;
                thunderBridge.printPluginStatus("RDK App Manager");
                break;
            case 2:
                cout << "Moving to App Manager related functions..." << endl;
                thunderBridge.showAppManagerMenu();
                choice = -1; // Reset choice to avoid exiting
                break;
            case 3:
                thunderBridge.showPackageManagerMenu();
                choice = -1; // Reset choice to avoid exiting
                break;
            case 0:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
int main() {
    thunderBridge.initialize();
    thunderBridge.connect();   
    processUserCommands();

    return 0;
}
