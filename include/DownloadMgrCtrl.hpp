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
    uint32_t AddRef() const
    {
        cout << " Hey I (PkgDownloaderEvtHandler::AddRef) am getting called  " << endl;
        return Core::ERROR_NONE;
    }
    uint32_t Release() const
    {
        cout << " Hey I (PkgDownloaderEvtHandler::Release) am getting called " << endl;
        return Core::ERROR_NONE;
    }
    void *QueryInterface(const uint32_t interfaceNumber)
    {
        cout << " Hey I (PkgDownloaderEvtHandler::QueryInterface) am getting called " << endl;
        if (interfaceNumber == Exchange::IPackageDownloader::INotification::ID)
        {
            return static_cast<Exchange::IPackageDownloader::INotification *>(this);
        }
        return nullptr;
    }
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