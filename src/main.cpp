#include <iostream>
#include "ThunderBridge.hpp"
using namespace std;

ThunderBridge thunderBridge;

void printMainMenu() {
    cout << "--------------------------" << endl;
    cout << "RDK App Managers Test Utility" << endl;
    cout << "Select an option:" << endl;
    cout << "1. Option 1 : Check RDK App managers status" << endl;
    cout << "2. Option 2 : Add Catalog url" << endl;
    cout << "3. Option 3 : List catalog applications" << endl;
    cout << "4: Option 4 : App Manager releated functions" << endl;
    cout << "5. Option 5 : Install application" << endl;
    cout << "6. Option 6 : Uninstall application" << endl;
    cout << "7. Option 7 : Update application" << endl;
    cout << "8. Option 8 : Launch application" << endl;
    cout << "9. Option 9 : Stop application" << endl;
    cout << "10. Option 10 : Get application status" << endl;
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
                cout << "Adding Catalog url..." << endl;
                // Add logic to add Catalog url
                break;
            case 3:
                cout << "Listing catalog applications..." << endl;
                // Add logic to list catalog applications
                break;
            case 4:
                cout << "Moving to App Manager related functions..." << endl;
                thunderBridge.showAppManagerMenu();
                choice = -1; // Reset choice to avoid exiting
                break;
            case 5:
                cout << "Installing application..." << endl;
                // Add logic to install application
                break;
            case 6:
                cout << "Uninstalling application..." << endl;
                // Add logic to uninstall application
                break;
            case 7:
                cout << "Updating application..." << endl;
                // Add logic to update application
                break;
            case 8:
                cout << "Launching application..." << endl;
                // Add logic to launch application
                break;
            case 9:
                cout << "Stopping application..." << endl;
                // Add logic to stop application
                break;
            case 10:
                cout << "Getting application status..." << endl;
                // Add logic to get application status
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
