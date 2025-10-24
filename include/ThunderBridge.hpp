#pragma once
#include "common.hpp"

#include "WPEFramework/interfaces/IPackageManager.h"
#include "AppMgrControl.hpp"
#include "PkgMgrControl.hpp"
#include "DownloadMgrCtrl.hpp"
#include "InstallMgrCtrl.hpp"


class ThunderBridge {
public:
    ThunderBridge();
    ~ThunderBridge();

    bool initialize();
    void connect();
    void deinitialize();
    void printPluginStatus(std::string pluginName) ;
    void showAppManagerMenu();
    void showPackageManagerMenu();

    bool initializeManager( MgrCtrl& manager);

private:
    unique_ptr<AppMgrControl> mAppMgrControl;
    unique_ptr<PkgMgrControl> mPkgMgrControl;
    unique_ptr<DownloadMgrControl> mDownloadMgrControl;
    unique_ptr<InstallMgrCtrl> mInstallMgrCtrl;
};
