#pragma once
#include <memory>
#include "common.hpp"
using namespace WPEFramework;
using namespace std;

class AppManagerEventHandler : public Exchange::IAppManager::INotification
{
public:
    ~AppManagerEventHandler() {}
    // Implement notification methods here
    void OnAppInstalled(const string &appId, const string &version)
    {
        std::cout << "App Installed: " << appId << " Version: " << version << std::endl;
    }
    void OnAppUninstalled(const string &appId)
    {
        std::cout << "App Uninstalled: " << appId << std::endl;
    }
    void OnAppLifecycleStateChanged(const string &appId, const string &appInstanceId, const Exchange::IAppManager::AppLifecycleState newState, const Exchange::IAppManager::AppLifecycleState oldState, const Exchange::IAppManager::AppErrorReason errorReason)
    {
        std::cout << "App Lifecycle State Changed: " << appId << " to state " << newState << std::endl;
    }
    void OnAppLaunchRequest(const string &appId, const string &intent, const string &source)
    {
        std::cout << "App Launch Request: " << appId << " Intent: " << intent << " Source: " << source << std::endl;
    }

    void OnAppUnloaded(const string &appId, const string &appInstanceId)
    {

        std::cout << "App Unloaded: " << appId << " Instance ID: " << appInstanceId << std::endl;
    }
    uint32_t AddRef() const
    {
        cout << " Hey I (AddRef) am getting called  " << endl;
        return Core::ERROR_NONE;
    }
    uint32_t Release() const
    {
        cout << " Hey I (Release) am getting called " << endl;
        return Core::ERROR_NONE;
    }
    void *QueryInterface(const uint32_t interfaceNumber)
    {
        cout << " Hey I (QueryInterface) am getting called " << endl;
        if (interfaceNumber == Exchange::IAppManager::INotification::ID)
        {
            return static_cast<Exchange::IAppManager::INotification *>(this);
        }
        return nullptr;
    }
};


class AppMgrControl
{
private:
    Exchange::IAppManager *appManager;
    shared_ptr<Exchange::IAppManager::INotification> appMgrEvtHandler = nullptr;

    void listInstalledApplications();


public:
    AppMgrControl(/* args */);
    ~AppMgrControl();
    bool initialize(Core::ProxyType<RPC::CommunicatorClient> &client);
    void checkPluginStatus();
    void displayAppManagerMenu();
    
};
