#include "InstallMgrCtrl.hpp"

InstallMgrCtrl::InstallMgrCtrl() : instlCtl(nullptr), instlEventHandler(nullptr)
{
}

InstallMgrCtrl::~InstallMgrCtrl()
{
    if (instlCtl != nullptr) {
        instlCtl->Release();
        instlCtl = nullptr;
    }
    instlEventHandler = nullptr;
}

bool InstallMgrCtrl::initialize(Core::ProxyType<RPC::CommunicatorClient> &client)
{
    instlCtl =  client->Open<Exchange::IPackageInstaller>("org.rdk.PackageManagerRDKEMS");
    if (instlCtl == nullptr) {
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
    if (instlCtl == nullptr) {
        std::cerr << "IPackageInstaller interface is not initialized." << std::endl;
        return false;
    }
    // Add actual status checking logic here
    return true;
}

void InstallMgrCtrl::displayMenu()
{
    std::cout << "Install Manager Control Menu:" << std::endl;
    std::cout << "1. Start Installation" << std::endl;
    std::cout << "2. Pause Installation" << std::endl;
    std::cout << "3. Resume Installation" << std::endl;
    std::cout << "4. Cancel Installation" << std::endl;
    std::cout << "5. Check Installation Status" << std::endl;
    std::cout << "6. Exit" << std::endl;
}