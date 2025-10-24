#include "ThunderBridge.hpp"
#include <iostream>
using namespace std;

#define comrpcPath "/tmp/communicator"

ThunderBridge::ThunderBridge()
{

    mAppMgrControl = std::unique_ptr<AppMgrControl>(new AppMgrControl());
    mPkgMgrControl = std::unique_ptr<PkgMgrControl>(new PkgMgrControl());
    mDownloadMgrControl = std::unique_ptr<DownloadMgrControl>(new DownloadMgrControl());
    mInstallMgrCtrl = std::unique_ptr<InstallMgrCtrl>(new InstallMgrCtrl());
}
ThunderBridge::~ThunderBridge()
{
    deinitialize();
}


bool ThunderBridge::initializeManager(MgrCtrl &manager)
{
    Core::ProxyType<RPC::CommunicatorClient> client;
    const char *thunderAccess = std::getenv("THUNDER_ACCESS");
    std::string envThunderAccess = (thunderAccess != nullptr) ? thunderAccess : comrpcPath;
    std::cout << "Using THUNDER_ACCESS: " << envThunderAccess << std::endl;

    Core::SystemInfo::SetEnvironment(_T("THUNDER_ACCESS"), envThunderAccess.c_str());
    client = Core::ProxyType<RPC::CommunicatorClient>::Create(
        Core::NodeId(envThunderAccess.c_str()));

    if (client.IsValid())
    {
        return manager.initialize(client);
    }
}
bool ThunderBridge::initialize()
{
    if (!initializeManager(*mAppMgrControl))
    {
        std::cerr << "Failed to initialize AppMgrControl." << std::endl;
        return false;
    }

    if (!initializeManager(*mPkgMgrControl))
    {
        std::cerr << "Failed to initialize PkgMgrControl." << std::endl;
        return false;
    }

    cout << "ThunderBridge initialized." << endl;
    return true;
}
void ThunderBridge::connect()
{

    cout << "Connected to Thunder framework." << endl;
}
void ThunderBridge::deinitialize()
{

    cout << "Disconnected from Thunder framework." << endl;
    mAppMgrControl.release();
    mPkgMgrControl.release();

    mAppMgrControl = nullptr;
    mPkgMgrControl = nullptr;
}
void ThunderBridge::printPluginStatus(std::string pluginName)
{

    cout << "Plugin Status for app manager: " << (mAppMgrControl ? (mAppMgrControl->checkPluginStatus() ? "Active" : "Not Active") : "AppManager is not initialized.") << endl;
    cout << "Plugin Status for package manager: " << (mPkgMgrControl ? (mPkgMgrControl->checkPluginStatus() ? "Active" : "Not Active") : "PkgManager is not initialized.") << endl;
}
void ThunderBridge::showAppManagerMenu()
{
    if (mAppMgrControl)
    {
        mAppMgrControl->displayMenu();
    }
    else
    {
        cout << "AppMgrControl is not initialized." << endl;
    }
}
void ThunderBridge::showPackageManagerMenu()
{
    if (mPkgMgrControl)
    {
        mPkgMgrControl->displayMenu();
    }
    else
    {
        cout << "PkgMgrControl is not initialized." << endl;
    }
}
