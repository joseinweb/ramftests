#include "PkgMgrControl.hpp"
PkgMgrControl::PkgMgrControl(/* args */) : pkgManager(nullptr)
{
}
PkgMgrControl::~PkgMgrControl()
{
    if (pkgManager)
    {
        pkgManager->Unregister(pkgMgrEvtHandler.get());
        pkgManager = nullptr;
    }
}
bool PkgMgrControl::initialize(Core::ProxyType<RPC::CommunicatorClient> &client)
{
    pkgManager =  client->Open<Exchange::IPackageManager>("org.rdk.PackageManagerRDKEMS");
    if (pkgManager == nullptr)
    {
        std::cout << "Failed to create PackageManager instance." << std::endl;
        return false;
    }

    pkgMgrEvtHandler = std::make_shared<PackageManagerEventHandler>();
    pkgManager->Register(pkgMgrEvtHandler.get());

    return true;
}
void PkgMgrControl::checkPluginStatus()
{
    if (pkgManager != nullptr)
    {
        std::cout << "PackageManager is initialized." << std::endl;
    }
}
void PkgMgrControl::displayMenu()
{
    int choice = -1;
    while (choice != 0)
    {
        std::cout << "\nPackage Manager Menu:\n";
        std::cout << "1. List Installed Packages\n";
        std::cout << "2. Install Package\n";
        std::cout << "3. Uninstall Package\n";
        std::cout << "4. Download Package\n";
        std::cout << "5. Reset Package\n";
        std::cout << "6. Get Storage Details\n";
        std::cout << "7. Set Package Auxiliary Details\n";
        std::cout << "8. Clear Package Auxiliary Details\n";
        std::cout << "9. Get Package Auxiliary Details\n";
        std::cout << "10. Cancel a request \n";
        std::cout << "11. Get Progress of a request \n";
        std::cout << "12. Lock Package \n";
        std::cout << "13. Unlock Package \n";
        std::cout << "14. Get Lock Details \n";

        std::cout << "0. Exit Package Manager Menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 0:
        default:
            std::cout << "Returning to main menu..." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
    }
}
