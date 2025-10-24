#include "InstallMgrCtrl.hpp"

InstallMgrCtrl::InstallMgrCtrl() : instlCtl(nullptr), instlEventHandler(nullptr)
{
}

InstallMgrCtrl::~InstallMgrCtrl()
{
    if (instlCtl != nullptr)
    {
        instlCtl->Release();
        instlCtl = nullptr;
    }
    instlEventHandler = nullptr;
}

bool InstallMgrCtrl::initialize(Core::ProxyType<RPC::CommunicatorClient> &client)
{
    instlCtl = client->Open<Exchange::IPackageInstaller>("org.rdk.PackageManagerRDKEMS");
    if (instlCtl == nullptr)
    {
        std::cerr << "Failed to open IPackageInstaller interface." << std::endl;
        return false;
    }

    instlEventHandler = std::make_shared<PkgInstallEvtHandler>();
    instlCtl->Register(instlEventHandler.get());
    client.Release();
    return true;
}

bool InstallMgrCtrl::checkPluginStatus()
{
    if (instlCtl == nullptr)
    {
        std::cerr << "IPackageInstaller interface is not initialized." << std::endl;
        return false;
    }
    // Add actual status checking logic here
    return true;
}

void InstallMgrCtrl::displayMenu()
{
    while (true)
    {
        std::cout << "Install Manager Control Menu:" << std::endl;
        std::cout << "1. Start Installation" << std::endl;
        std::cout << "2. Pause Installation" << std::endl;
        std::cout << "3. Resume Installation" << std::endl;
        std::cout << "4. Cancel Installation" << std::endl;
        std::cout << "5. Check Installation Status" << std::endl;
        std::cout << "0. Return to Main Menu" << std::endl;

        int choice = retrieveInputFromUser<int>("Enter your choice: ", false, 0);

        switch (choice)
        {
        case 1:
            // Add logic for starting installation
            break;
        case 2:
            // Add logic for pausing installation
            break;
        case 3:
            // Add logic for resuming installation
            break;
        case 4:
            // Add logic for canceling installation
            break;
        case 5:
            // Add logic for checking installation status
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}