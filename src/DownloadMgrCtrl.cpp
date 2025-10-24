#include "DownloadMgrCtrl.hpp"
#include <cassert>

DownloadMgrControl::DownloadMgrControl() : dwldCtl(nullptr), dwldEventHandler(nullptr)
{
}
DownloadMgrControl::~DownloadMgrControl()
{
    if (dwldCtl != nullptr)
    {
        dwldCtl->Release();
        dwldCtl = nullptr;
    }
    dwldEventHandler = nullptr;
}
bool DownloadMgrControl::initialize(Core::ProxyType<RPC::CommunicatorClient> &client)
{
    dwldCtl = client->Open<Exchange::IPackageDownloader>("org.rdk.PackageManagerRDKEMS");
    if (dwldCtl == nullptr)
    {
        std::cerr << "Failed to open IPackageDownloader interface." << std::endl;
        return false;
    }

    dwldEventHandler = std::make_shared<PkgDownloaderEvtHandler>();
    dwldCtl->Register(dwldEventHandler.get());
    client.Release();
    return true;
}
bool DownloadMgrControl::checkPluginStatus()
{
    return (dwldCtl != nullptr);
}

void DownloadMgrControl::displayMenu()
{
    while (true)
    {
        std::cout << "Download Manager Control Menu:" << std::endl;
        std::cout << "1. Start Download" << std::endl;
        std::cout << "2. Pause Download" << std::endl;
        std::cout << "3. Resume Download" << std::endl;
        std::cout << "4. Cancel Download" << std::endl;
        std::cout << "5. Check Download Status" << std::endl;
        std::cout << "6. Check Download progress" << std::endl;
        std::cout << "7. Get Storage Details" << std::endl;
        std::cout << "8. Set RateLimit" << std::endl;
        std::cout << "0. Return to Main Menu" << std::endl;


        int choice = retrieveInputFromUser<int>("Enter your choice: ", false, 0);


        switch (choice)
        {
        case 1:
            handleStartDownloadRequest();
            break;
        case 2:
            handlePauseDownloadRequest();
            break;
        case 3:
            handleResumeDownloadRequest();
            break;
        case 4:
            handleCancelDownloadRequest();
            break;
        case 5:
            handleCheckDownloadStatusRequest();
            break;
        case 6:
            handleCheckDownloadProgressRequest();
            break;
        case 7:
            handleGetStorageDetailsRequest();
            break;
        case 8:
            handleSetRateLimitRequest();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}
void DownloadMgrControl::handleStartDownloadRequest()
{
    assert(dwldCtl != nullptr && "IPackageDownloader interface is not initialized.");

    std::string url = retrieveInputFromUser<std::string>("Enter Download URL: ", false, "");
    bool highPriority = retrieveInputFromUser<bool>("Is this a high priority download? (true/false): ", true, false);
    int maxSpeed = retrieveInputFromUser<int>("Enter maximum download speed (KB/s, 0 for unlimited): ", true, 0);
    int retries  = retrieveInputFromUser<int>("Enter number of retries: default 2 ", true, 2);

    Exchange::IPackageDownloader::Options params;
    params.priority = highPriority;
    params.rateLimit = maxSpeed;
    params.retries = retries;

    Exchange::IPackageDownloader::DownloadId downloadId;

    // Call the appropriate method on dwldCtl to start the download
    uint32_t result = dwldCtl->Download(url, params, downloadId);
    if (result != Core::ERROR_NONE)
    {
        std::cerr << "Failed to start download: " << url << std::endl;
        return;
    }
    std::cout << "Download started successfully. Download ID: " << downloadId.downloadId << std::endl;
}

void DownloadMgrControl::handlePauseDownloadRequest()
{
}
void DownloadMgrControl::handleResumeDownloadRequest()
{
}
void DownloadMgrControl::handleCancelDownloadRequest()
{
}
void DownloadMgrControl::handleCheckDownloadStatusRequest()
{
}
void DownloadMgrControl::handleCheckDownloadProgressRequest()
{
}
void DownloadMgrControl::handleGetStorageDetailsRequest()
{
}
void DownloadMgrControl::handleSetRateLimitRequest()
{
}