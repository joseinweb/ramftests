#pragma once

#ifndef MODULE_NAME
#define MODULE_NAME AppMgrsTest
#endif

#include <WPEFramework/com/com.h>
#include <WPEFramework/core/core.h>
#include "WPEFramework/interfaces/IAppManager.h"
#include "WPEFramework/interfaces/IPackageManager.h"

using namespace WPEFramework;

class ThunderBridge {
public:
    ThunderBridge();
    ~ThunderBridge();

    bool initialize();
    void connect();
    void deinitialize();
    void printPluginStatus(std::string pluginName) ;

private:

    Exchange::IAppManager *mAppManager;
    Core::ProxyType<RPC::CommunicatorClient> mClient;
    std::shared_ptr<WPEFramework::Exchange::IAppManager::INotification> mAppManagerEventHandler;
};
