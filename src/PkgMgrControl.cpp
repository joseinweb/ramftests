#include "PkgMgrControl.hpp"
PkgMgrControl::PkgMgrControl() : pkgCtrl(nullptr)
{
}
PkgMgrControl::~PkgMgrControl()
{
    if (pkgCtrl)
    {

        pkgCtrl->Release();
        pkgCtrl = nullptr;
    }
}
bool PkgMgrControl::initialize(Core::ProxyType<RPC::CommunicatorClient> &client)
{

    pkgCtrl = client->Open<Exchange::IPackageHandler>("org.rdk.PackageManagerRDKEMS");
    if (pkgCtrl == nullptr)
    {
        std::cout << "Failed to create PackageHandler instance." << std::endl;
        return false;
    }
    client.Release();
    return true;
}
bool PkgMgrControl::checkPluginStatus()
{
    return (pkgCtrl != nullptr);
}
void PkgMgrControl::displayMenu()
{
    int choice = -1;
    while (choice != 0)
    {

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
