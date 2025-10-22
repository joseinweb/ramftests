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

        std::cout << "--------------------------" << std::endl;
        std::cout << "App Manager Menu:" << std::endl;
        std::cout << "0. Go back to main menu" << std::endl;
        std::cout << "1. List Installed Applications" << std::endl;
        std::cout << "2. Is App installed ? " << std::endl;
        std::cout << "3. List Loaded apps" << std::endl;
        std::cout << "4. Launch Application" << std::endl;
        std::cout << "5. Preload Application" << std::endl;
        std::cout << "6. Close Application" << std::endl;
        std::cout << "7. Terminate Application" << std::endl;
        std::cout << "8. Kill Application" << std::endl;
        std::cout << "9. Start System app" << std::endl;
        std::cout << "10. Stop System app" << std::endl;
        std::cout << "11. Send App Intent" << std::endl;
        std::cout << "12. Clear application data" << std::endl;
        std::cout << "13. Clear all application data" << std::endl;
        std::cout << "14. Get application metadata" << std::endl;
        std::cout << "15. Get application property" << std::endl;
        std::cout << "16. Set application property" << std::endl;
        std::cout << "17. Get Max running applications" << std::endl;
        std::cout << "18. Get Max Hibernated applications" << std::endl;
        std::cout << "19. Get Min Hibernated flash usage" << std::endl;
        std::cout << "20. Get Max inactive RAM usage" << std::endl;
        std::cout << "--------------------------" << std::endl;
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
            handleIsAppInstalledRequest();
            break;
        case 3:
            handleLoadedAppsRequest();
            break;
        case 4:
            handleLaunchApplicationRequest();
            break;
        case 5:
            // handlePreloadApplicationRequest();
            break;
        case 6:
            handleCloseApplicationRequest();
            break;
        case 7:
            handleTerminateApplicationRequest();
            break;
        case 8:
            handleKillApplicationRequest();
            break;

        case 0:
        default:
            std::cout << "Returning to main menu..." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
    }
}
void AppMgrControl::handleApplicationClosureRequest(CLOSURE_REASON reason)
{
    if (appManager == nullptr)
    {
        std::cerr << "AppManager is not initialized." << std::endl;
        return;
    }

    std::string appInstanceId;
    std::cout << "Enter the App Instance ID to handle closure: ";
    std::cin >> appInstanceId;
    uint32_t result = 0;
    switch (reason)
    {
    case CLOSURE_REASON::CLOSE:
        result = appManager->CloseApp(appInstanceId);
        break;
    case CLOSURE_REASON::TERMINATE:
        result = appManager->TerminateApp(appInstanceId);
        break;
    case CLOSURE_REASON::KILL:
        result = appManager->KillApp(appInstanceId);
        break;
    default:
        std::cerr << "Invalid closure reason." << std::endl;
        return;
    }

    if (result != Core::ERROR_NONE)
    {
        std::cerr << "Failed to handle application closure: " << appInstanceId << std::endl;
        return;
    }
    std::cout << "Handle closure request sent for application instance: " << appInstanceId << std::endl;
}
void AppMgrControl::handleCloseApplicationRequest()
{
    handleApplicationClosureRequest(CLOSURE_REASON::CLOSE);
}
void AppMgrControl::handleTerminateApplicationRequest()
{
    handleApplicationClosureRequest(CLOSURE_REASON::TERMINATE);
}
void AppMgrControl::handleKillApplicationRequest()
{
    handleApplicationClosureRequest(CLOSURE_REASON::KILL);
}
void AppMgrControl::handleLaunchApplicationRequest()
{
    if (appManager == nullptr)
    {
        std::cerr << "AppManager is not initialized." << std::endl;
        return;
    }

    std::string appId;
    std::cout << "Enter the App ID to launch: ";
    std::cin >> appId;

    uint32_t result = appManager->LaunchApp(appId, "", "");

    if (result != Core::ERROR_NONE)
    {
        std::cerr << "Failed to launch application: " << appId << std::endl;
        return;
    }
    std::cout << "Launch request sent for application: " << appId << std::endl;
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
    if (!reader->parse(apps.c_str(), apps.c_str() + apps.size(), &root, &err))
    {
        std::cerr << "Failed to parse installed applications JSON: " << err << std::endl;
        return;
    }
    std::cout << "Installed Applications:" << std::endl;
    for (const auto &app : root)
    {
        std::cout << " - " << app["appId"].asString() << " (Version: " << app["versionString"].asString() << ")"
                  << " Last active: " << app["lastActiveTime"].asString() << std::endl;
    }
}
void AppMgrControl::handleIsAppInstalledRequest()
{
    if (appManager == nullptr)
    {
        std::cerr << "AppManager is not initialized." << std::endl;
        return;
    }

    std::string appId;
    std::cout << "Enter the App ID to check: ";
    std::cin >> appId;

    bool isInstalled = false;
    uint32_t result = appManager->IsInstalled(appId, isInstalled);

    if (result != Core::ERROR_NONE)
    {
        std::cerr << "Failed to check if app is installed." << std::endl;
        return;
    }
    std::cout << appId << " is  ?" << (isInstalled ? "Installed" : "Not Installed") << std::endl;
}
void AppMgrControl::handleLoadedAppsRequest()
{
    if (appManager == nullptr)
    {
        std::cerr << "AppManager is not initialized." << std::endl;
        return;
    }
    std::string loadedAppsJson;

    uint32_t result = appManager->GetLoadedApps(loadedAppsJson);
    if (result == Core::ERROR_NONE)
    {
        Json::CharReaderBuilder builder;
        Json::Value root;
        JSONCPP_STRING err;

        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        if (!reader->parse(loadedAppsJson.c_str(), loadedAppsJson.c_str() + loadedAppsJson.size(), &root, &err))
        {
            std::cerr << "Failed to parse loaded applications JSON: " << err << std::endl;
            return;
        }

        std::cout << "Loaded Applications:" << std::endl;
        for (const auto &app : root)
        {
            std::cout << " - " << app["appId"].asString() << " (appInstanceId: " << app["appInstanceId"].asString() << ")"
                      << " activeSessionId: " << app["activeSessionId"].asString() << " Current state: " << mapLifeCycleStateToString(static_cast<Exchange::IAppManager::AppLifecycleState>(app["currentLifecycleState"].asInt())) << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to retrieve loaded applications." << std::endl;
    }
}
