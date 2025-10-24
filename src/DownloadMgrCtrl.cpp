#include "DownloadMgrCtrl.hpp"

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
    if (dwldCtl == nullptr)
    {
        std::cerr << "IPackageDownloader interface is not initialized." << std::endl;
        return false;
    }
    // Add actual status checking logic here
    return true;
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
        int choice = -1;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

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
        default:
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
    }
}
void DownloadMgrControl::handleStartDownloadRequest()
{
    if (nullptr != dwldCtl)
    {
        std::cout << "Enter Download URL: ";
        std::string url;
        std::cin >> url;

        std::cout << "High priority download? (y/n): ";
        char priority;
        std::cin >> priority;
        bool highPriority = (priority == 'y' || priority == 'Y');

        std::cout << "Enter maximum download speed (KB/s, 0 for unlimited): ";
        int maxSpeed;
        std::cin >> maxSpeed;

        std::cout << "Enter number of retries: ";
        int retries;
        std::cin >> retries;

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
}

    void DownloadMgrControl::handlePauseDownloadRequest(){

    }
    void DownloadMgrControl::handleResumeDownloadRequest(){

    }
    void DownloadMgrControl::handleCancelDownloadRequest(){
    }
    void DownloadMgrControl:: handleCheckDownloadStatusRequest(){

    }
    void DownloadMgrControl::handleCheckDownloadProgressRequest(){

    }
    void DownloadMgrControl::handleGetStorageDetailsRequest(){
        
    }
    void DownloadMgrControl::handleSetRateLimitRequest(){
    }