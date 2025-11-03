#include <iostream>
#include "ThunderBridge.hpp"
#include "CatalogManager.hpp"
using namespace std;

ThunderBridge thunderBridge;
CatalogManager catalogManager;

void printMainMenu()
{
    cout << "--------------------------" << endl;
    cout << "RDK App Managers Test Utility" << endl;
    cout << "Select an option:" << endl;

    cout << "1. Check RDK App managers status" << endl;
    cout << "2. App Manager related functions" << endl;
    cout << "3. Package Manager related functions" << endl;
    cout << "4. Perform Catalog retrieval" << endl;
    cout << "0 . Exit" << endl;
    cout << "--------------------------" << endl;
}

int processUserCommands()
{

    while (true)
    {
        printMainMenu();

        int choice = retrieveInputFromUser<int>("Enter your choice: ", false, 0);
        switch (choice)
        {
        case 1:
            cout << "Checking RDK App managers status..." << endl;
            thunderBridge.printPluginStatus("RDK App Manager");
            break;
        case 2:
            thunderBridge.showAppManagerMenu();
            break;
        case 3:
            thunderBridge.showPackageManagerMenu();
            break;
        case 4:
            catalogManager.displayMenu();
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
int main()
{
    thunderBridge.initialize();
    thunderBridge.connect();
    processUserCommands();

    return 0;
}
