#pragma once
#include "common.hpp"

#include "WPEFramework/interfaces/IPackageManager.h"
#include "AppMgrControl.hpp"
using namespace WPEFramework;

class ThunderBridge {
public:
    ThunderBridge();
    ~ThunderBridge();

    bool initialize();
    void connect();
    void deinitialize();
    void printPluginStatus(std::string pluginName) ;
    void showAppManagerMenu();

private:

    Core::ProxyType<RPC::CommunicatorClient> mClient;
    unique_ptr<AppMgrControl> mAppMgrControl;
};
