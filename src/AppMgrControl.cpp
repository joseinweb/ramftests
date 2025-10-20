#include "AppMgrControl.hpp"
#include "json/json.h"

AppMgrControl::AppMgrControl(/* args */)
{
    appManager = nullptr;
    appMgrEvtHandler = nullptr;
}

AppMgrControl::~AppMgrControl()
{
}
bool AppMgrControl::initialize(Core::ProxyType<RPC::CommunicatorClient> &client)
{
    appManager = client->Open<Exchange::IAppManager>("org.rdk.AppManager");
    if (appManager == nullptr)
    {
        std::cerr << "Failed to open IAppManager interface." << std::endl;
        return false;
    }

    appMgrEvtHandler = std::make_shared<AppManagerEventHandler>();
    appManager->Register(appMgrEvtHandler.get());

    return true;
}
void AppMgrControl::checkPluginStatus()
{
    if (appManager != nullptr)
    {
        std::cout << "AppManager is initialized." << std::endl;
    }
}
void AppMgrControl::displayAppManagerMenu()
{
    while (true)
    {

        std::cout << "App Manager Menu:" << std::endl;
        std::cout << "0. Go back to main menu" << std::endl;
        std::cout << "1. List Installed Applications" << std::endl;
        std::cout << "2. Other App Manager Functions" << std::endl;
        // Add more menu options as needed

        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "Listing Installed Applications..." << std::endl;
            listInstalledApplications();
            break;
        case 2:
            std::cout << "Other App Manager Functions..." << std::endl;
            // Add logic for other app manager functions
            break;
        case 0:
            std::cout << "Returning to main menu..." << std::endl;
            return;
        default:
            std::cout << "Invalid choice: " << choice << std::endl;
        }
    }
}
void AppMgrControl::listInstalledApplications()
{
    if (appManager == nullptr)
    {
        std::cerr << "AppManager is not initialized." << std::endl;
        return;
    }

    std::string apps;
    uint32_t result = appManager->GetInstalledApps(apps);
    if (result != Core::ERROR_NONE)
    {
        std::cerr << "Failed to retrieve installed applications." << std::endl;
        return;
    }
    Json::CharReaderBuilder builder;
    Json::Value root;
    JSONCPP_STRING err;
    
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if(!reader->parse(apps.c_str(), apps.c_str() + apps.size(), &root, &err)) {
        std::cerr << "Failed to parse installed applications JSON: " << err << std::endl;
        return;
    }
    std::cout << "Installed Applications:" << std::endl;
    for (const auto& app : root)
    {
        std::cout << " - " << app["appId"].asString() << " (Version: " << app["versionString"].asString() << ")" 
        <<" Last active: " << app["lastActiveTime"].asString() << std::endl;
    }
}