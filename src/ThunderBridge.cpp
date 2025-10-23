#include "ThunderBridge.hpp"
#include <iostream>
using namespace std;

#define comrpcPath "/tmp/communicator"

ThunderBridge::ThunderBridge()
{

    mAppMgrControl = std::unique_ptr<AppMgrControl>(new AppMgrControl());
}
ThunderBridge::~ThunderBridge()
{
    deinitialize();
}
bool ThunderBridge::initialize()
{
    const char *thunderAccess = std::getenv("THUNDER_ACCESS");
    std::string envThunderAccess = (thunderAccess != nullptr) ? thunderAccess : comrpcPath;
    std::cout << "Using THUNDER_ACCESS: " << envThunderAccess << std::endl;

    Core::SystemInfo::SetEnvironment(_T("THUNDER_ACCESS"), envThunderAccess.c_str());
    mClient = Core::ProxyType<RPC::CommunicatorClient>::Create(
        Core::NodeId(envThunderAccess.c_str()));

    if (mClient.IsValid()) {
        cout << " Registered to Thunder" << endl;
    } else {
        std::cerr << "Failed to create COMRPC client." << std::endl;
        return false;
    }
    cout << "ThunderBridge initialized." << endl;
    if(!mAppMgrControl->initialize(mClient)) {
        std::cerr << "Failed to initialize AppMgrControl." << std::endl;
        return false;
    }
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
    mAppMgrControl = nullptr;
    mClient.Release();
}
void ThunderBridge::printPluginStatus(std::string pluginName)
{
    cout<<"Plugin Status for " << pluginName << ": ";
    if(mAppMgrControl) {
        mAppMgrControl->checkPluginStatus();
    } else {
        cout<<"AppMgrControl is not initialized."<<endl;
    }
}
void ThunderBridge::showAppManagerMenu()
{
    if(mAppMgrControl) {
        mAppMgrControl->displayMenu();
    } else {
        cout<<"AppMgrControl is not initialized."<<endl;
    }
}
