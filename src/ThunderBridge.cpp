#include "ThunderBridge.hpp"
#include <iostream>
using namespace std;

#define comrpcPath "/tmp/communicator"
const std::string appmgrCallsign = "org.rdk.AppManager";
ThunderBridge::ThunderBridge() : appManager(nullptr), appManagerEventHandler(nullptr)
{
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
        mAppManager = mClient->Open<Exchange::IAppManager>(appmgrCallsign.c_str());
        if (mAppManager == nullptr) {
            std::cerr << "Failed to open IAppManager interface." << std::endl;
            return false;
        }
    } else {
        std::cerr << "Failed to create COMRPC client." << std::endl;
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
}
void ThunderBridge::printPluginStatus(std::string pluginName)
{
    cout<<"Plugin Status for " << pluginName << ": ";
}
