#include "InstallMgrCtrl.hpp"
#include <cassert>

InstallMgrCtrl::InstallMgrCtrl() : instlCtl(nullptr), instlEventHandler(nullptr)
{
}

InstallMgrCtrl::~InstallMgrCtrl()
{
    if (instlCtl != nullptr)
    {
        instlCtl->Release();
        instlCtl = nullptr;
    }
    instlEventHandler = nullptr;
}

bool InstallMgrCtrl::initialize(Core::ProxyType<RPC::CommunicatorClient> &client)
{
    instlCtl = client->Open<Exchange::IPackageInstaller>("org.rdk.PackageManagerRDKEMS");
    if (instlCtl == nullptr)
    {
        std::cerr << "Failed to open IPackageInstaller interface." << std::endl;
        return false;
    }

    instlEventHandler = std::make_shared<PkgInstallEvtHandler>();
    instlCtl->Register(instlEventHandler.get());
    client.Release();
    return true;
}

bool InstallMgrCtrl::checkPluginStatus()
{
    return instlCtl != nullptr;
}

void InstallMgrCtrl::displayMenu()
{
    while (true)
    {
        std::cout << "------------------------------------------------------------" << std::endl;
        std::cout << "Install Manager Control Menu:" << std::endl;
        std::cout << "1. Install Package" << std::endl;
        std::cout << "2. Uninstall Package" << std::endl;
        std::cout << "3. List Packages" << std::endl;
        std::cout << "4. Package install state " << std::endl;
        std::cout << "5. Package Metadata " << std::endl;
        std::cout << "6. Package  Configuration " << std::endl;
        std::cout << "0. Return to Main Menu" << std::endl;

        int choice = retrieveInputFromUser<int>("Enter your choice: ", false, 0);
        std::cout << "------------------------------------------------------------" << std::endl;

        switch (choice)
        {
        case 1:
            handleStartInstallRequest();
            break;
        case 2:
            handleUninstallRequest();
            break;
        case 3:
            handleListPackagesRequest();
            break;
        case 4:
            handlePackageInstallStateRequest();

            break;
        case 5:
            handlePackageMetadataRequest();
            break;
        case 6:
            handlePackageConfigurationRequest();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

void InstallMgrCtrl::handleStartInstallRequest()
{
    assert(instlCtl != nullptr && "IPackageInstaller interface is not initialized.");

    std::string packageId = retrieveInputFromUser<std::string>("Enter the package Id to install: ", false, "");
    std::string version = retrieveInputFromUser<std::string>("Enter the package version to install : ", false, "");
    std::string fileLocator = retrieveInputFromUser<std::string>("Enter the file locator : ", false, "");

    // "params": {"packageId": "com.rdk.app.wpebrowser_2.38", "version": "2025", "fileLocator": "/opt/data/com.rdk.app.wpebrowser_2.38_1.0.0-b34e9a38a2675d4cd02cf89f7fc72874a4c99eb0-dbg.tgz",
    //"additionalMetadata": [{"name": "appName", "value": "WPEBrowser_2.38"}, {"name": "category", "value": "Application"}, {"name": "type", "value": "application/dac.native"}]}}

    std::string appName = retrieveInputFromUser<std::string>("Enter appname : ", false, "");
    std::string category = retrieveInputFromUser<std::string>("Enter category : ", true, "Application");
    std::string type = retrieveInputFromUser<std::string>("Enter type : ", true, "application/dac.native");

    std::list<Exchange::IPackageInstaller::KeyValue> additionalMetadata;
    additionalMetadata.push_back({"appName", appName});
    additionalMetadata.push_back({"category", category});
    additionalMetadata.push_back({"type", type});

    // using IKeyValueIterator = RPC::IIteratorType<Exchange::IPackageInstaller::KeyValue, Exchange::ID_PACKAGE_KEY_VALUE_ITERATOR>;
    // IKeyValueIterator* metadataIterator = Core::Service<IKeyValueIterator>::Create<IKeyValueIterator>(additionalMetadata);
    uint32_t result = Core::ERROR_NONE;
    Exchange::IPackageInstaller::FailReason failReason;
    // uint32_t result = instlCtl->Install(packageId, version, metadataIterator,fileLocator,failReason);
    if (result == Core::ERROR_NONE)
    {
        std::cout << "Installation started successfully." << std::endl;
    }
    else
    {
        std::cout << "Installation failed to start. Reason: " << mapFailureReason(failReason) << std::endl;
    }

    //metadataIterator->Release();
}
void InstallMgrCtrl::handleUninstallRequest()
{
    assert(instlCtl != nullptr && "IPackageInstaller interface is not initialized.");
    std::string packageId = retrieveInputFromUser<std::string>("Enter the package Id to uninstall: ", false, "");

    std::string errorReason;

    uint32_t result = instlCtl->Uninstall(packageId, errorReason);

    if (result == Core::ERROR_NONE)
    {
        std::cout << "Uninstallation successful." << std::endl;
    }
    else
    {
        std::cout << "Uninstallation failed. Reason: " << errorReason << std::endl;
    }
}
void InstallMgrCtrl::handleListPackagesRequest()
{
    assert(instlCtl != nullptr && "IPackageInstaller interface is not initialized.");

    using IPackageIterator = RPC::IIteratorType<Exchange::IPackageInstaller::Package, Exchange::ID_PACKAGE_ITERATOR>;
    IPackageIterator *packageItr;
    uint32_t result = instlCtl->ListPackages(packageItr);

    if (result == Core::ERROR_NONE)
    {

        std::cout << "Installed Packages:" << std::endl;
        Exchange::IPackageInstaller::Package pkg;
        while (packageItr->Next(pkg))
        {
            std::cout << "- " << pkg.packageId << " , Version: " << pkg.version
                      << ", State " << pkg.state << ", Digest " << pkg.digest << ", sizeKB: " << pkg.sizeKb << std::endl;
        }
        packageItr->Release();
    }
    else
    {
        std::cout << "Failed to list packages." << std::endl;
    }
}
void InstallMgrCtrl::handlePackageInstallStateRequest()
{
    assert(instlCtl != nullptr && "IPackageInstaller interface is not initialized.");
    std::string packageId = retrieveInputFromUser<std::string>("Enter the package Id to get install state: ", false, "");
    std::string version = retrieveInputFromUser<std::string>("Enter the package version: ", false, "");

    Exchange::IPackageInstaller::InstallState installState;
    uint32_t result = instlCtl->PackageState(packageId, version, installState);

    if (result == Core::ERROR_NONE)
    {
        std::cout << "Package Install State: " << to_string(installState) << std::endl;
    }
    else
    {
        std::cout << "Failed to get package install state." << std::endl;
    }
}

void InstallMgrCtrl::handlePackageMetadataRequest()

{
    assert(instlCtl != nullptr && "IPackageInstaller interface is not initialized.");
    std::string packageId = retrieveInputFromUser<std::string>("Enter the package Id to get metadata: ", false, "");
    std::string version = retrieveInputFromUser<std::string>("Enter the package version: ", false, "");

    Exchange::RuntimeConfig rtConfig;

    uint32_t result = instlCtl->Config(packageId, version, rtConfig);

    if (result == Core::ERROR_NONE)
    {
        std::cout << "Package Metadata retrieved successfully." << std::endl;
        // TODO: Display or process the metadata as needed
    }
    else
    {
        std::cout << "Failed to get package metadata." << std::endl;
    }
}

void InstallMgrCtrl::handlePackageConfigurationRequest()
{
    assert(instlCtl != nullptr && "IPackageInstaller interface is not initialized.");
    std::string packageId = retrieveInputFromUser<std::string>("Enter the package Id to get metadata: ", false, "");
    std::string fileLocator = retrieveInputFromUser<std::string>("Enter the file locator: ", false, "");
    std::string version = retrieveInputFromUser<std::string>("Enter the package version: ", false, "");

    Exchange::RuntimeConfig config;

    uint32_t result = instlCtl->GetConfigForPackage(fileLocator, packageId, version, config);

    if (result == Core::ERROR_NONE)
    {
        std::cout << "Package Configuration retrieved successfully." << std::endl;
        // TODO: Display or process the configuration as needed
    }
    else
    {
        std::cout << "Failed to get package configuration." << std::endl;
    }
}