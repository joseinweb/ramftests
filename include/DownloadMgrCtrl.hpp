#pragma once
#include "common.hpp"
#include "WPEFramework/interfaces/IAppPackageManager.h"
#include "MgrControl.hpp"

class PkgDownloaderEvtHandler : public Exchange::IPackageDownloader::INotification
{
    using IPackageInfoIterator = RPC::IIteratorType<Exchange::IPackageDownloader::PackageInfo, Exchange::ID_PACKAGE_INFO_ITERATOR>;

public:
    ~PkgDownloaderEvtHandler() {}
    void OnAppDownloadStatus(IPackageInfoIterator *const packageInfo)
    {
        Exchange::IPackageDownloader::PackageInfo info;
        while (packageInfo->Next(info))
        {
            cout << "Package: " << info.downloadId << ", Locator: " << info.fileLocator << ", Reason : " << info.reason << endl;
        }
        packageInfo->Release();
    }
    BEGIN_INTERFACE_MAP(PkgDownloaderEvtHandler)
    INTERFACE_ENTRY(Exchange::IPackageDownloader::INotification)
    END_INTERFACE_MAP

};

class DownloadMgrControl : public MgrCtrl
{
private:
    Exchange::IPackageDownloader *dwldCtl;
    shared_ptr<Exchange::IPackageDownloader::INotification> dwldEventHandler = nullptr;

    void handleStartDownloadRequest();
    void handlePauseDownloadRequest();
    void handleResumeDownloadRequest();
    void handleCancelDownloadRequest();
    void handleCheckDownloadProgressRequest();
    void handleDeleteInstallerFileRequest();
    void handleGetStorageDetailsRequest();
    void handleSetRateLimitRequest();

public:
    DownloadMgrControl();
    ~DownloadMgrControl();
    bool initialize(Core::ProxyType<RPC::CommunicatorClient> &client) override;
    bool checkPluginStatus() override;
    void displayMenu() override;
};