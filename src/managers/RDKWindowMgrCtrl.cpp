#include "RDKWindowMgrCtrl.hpp"
#include <fstream>

void writePNG(const char *filename, const std::string &imageData)
{
    // This function should implement the logic to write PNG data to a file.
    // For demonstration purposes, we will just write the raw data to a file.
    // In a real implementation, you would need to use a PNG library like libpng to properly format the PNG file.

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile)
    {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    // TODO This is just a dump. We need to make it proper format.
    outFile.write(pngHeader, sizeof(pngHeader));
    outFile.write(imageData.c_str(), imageData.size());
    outFile.close();
}

RDKWindowMgrCtrl::RDKWindowMgrCtrl() : MgrCtrl()
{
    // Constructor implementation
}

RDKWindowMgrCtrl::~RDKWindowMgrCtrl()
{
    // Destructor implementation
}

bool RDKWindowMgrCtrl::initialize(Core::ProxyType<RPC::CommunicatorClient> &client)
{
    // Initialize the IRDKWindowManager interface
    windowMgrCtrl = client->Open<Exchange::IRDKWindowManager>("org.rdk.RDKWindowManager");
    if (windowMgrCtrl == nullptr)
    {
        std::cout << "Failed to create RDKWindowManager instance." << std::endl;
        return false;
    }
    client.Release();

    // Set up event handler for RDKWindowManager notifications
    wMgrEventHandler = std::make_shared<WMgrEvtHandler>();
    windowMgrCtrl->Register(wMgrEventHandler.get());
    return true;
}
bool RDKWindowMgrCtrl::checkPluginStatus()
{
    return (windowMgrCtrl != nullptr);
}
void RDKWindowMgrCtrl::displayMenu()
{

    while (true)
    {

        std::cout << "RDK Window Manager Menu" << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;
        std::cout << "Enter your choice: \n";
        std::cout << "1. Get Clients\n";
        std::cout << "2. SetVisibility\n";
        std::cout << "3. SetFocus \n";
        std::cout << "4. GetVisibility\n";
        std::cout << "5. GetScreenshot\n";
        std::cout << "0. Exit Package Manager Menu\n";

        int choice = retrieveInputFromUser<int>("Enter your choice: ", false, 0);
        std::cout << "------------------------------------------------------------" << std::endl;

        switch (choice)
        {
        case 1:
            handleGetClientsRequest();
            break;
        case 2:
            handleSetVisibilityRequest();
            break;
        case 3:
            handleSetFocusRequest();
            break;
        case 4:
            handleGetVisibilityRequest();
            break;
        case 5:
            handleGetScreenshotRequest();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}
// Handle each menu option with corresponding methods
void RDKWindowMgrCtrl::handleGetClientsRequest()
{
    assert(windowMgrCtrl != nullptr && "IRDKWindowManager interface is not initialized.");
    std::string clients;
    Core::hresult result = windowMgrCtrl->GetApps(clients);
    if (result == Core::ERROR_NONE)
    {
        std::cout << "Connected Clients: " << clients << std::endl;
    }
    else
    {
        std::cout << "Failed to retrieve clients. Error code: " << result << std::endl;
    }
}
void RDKWindowMgrCtrl::handleSetVisibilityRequest()
{
    assert(windowMgrCtrl != nullptr && "IRDKWindowManager interface is not initialized.");
    std::string clientName = retrieveInputFromUser<std::string>("Enter client name to set visibility: ", false, "");
    bool visibility = retrieveInputFromUser<bool>("Enter visibility (0 for false, 1 for true): ", false, true);
    Core::hresult result = windowMgrCtrl->SetVisible(clientName, visibility);
    if (result == Core::ERROR_NONE)
    {
        std::cout << "Visibility set successfully for client: " << clientName << std::endl;
    }
    else
    {
        std::cout << "Failed to set visibility. Error code: " << result << std::endl;
    }
}
void RDKWindowMgrCtrl::handleSetFocusRequest()
{
    assert(windowMgrCtrl != nullptr && "IRDKWindowManager interface is not initialized.");
    std::string clientName = retrieveInputFromUser<std::string>("Enter client name to set focus: ", false, "");
    Core::hresult result = windowMgrCtrl->SetFocus(clientName);
    if (result == Core::ERROR_NONE)
    {
        std::cout << "Focus set successfully for client: " << clientName << std::endl;
    }
    else
    {
        std::cout << "Failed to set focus. Error code: " << result << std::endl;
    }
}
void RDKWindowMgrCtrl::handleGetVisibilityRequest()
{
    assert(windowMgrCtrl != nullptr && "IRDKWindowManager interface is not initialized.");
    std::string clientName = retrieveInputFromUser<std::string>("Enter client name to get visibility: ", false, "");
    bool visibility;
    Core::hresult result = windowMgrCtrl->GetVisibility(clientName, visibility);
    if (result == Core::ERROR_NONE)
    {
        std::cout << "Visibility for client " << clientName << ": " << (visibility ? "Visible" : "Hidden") << std::endl;
    }
    else
    {
        std::cout << "Failed to get visibility. Error code: " << result << std::endl;
    }
}
void RDKWindowMgrCtrl::handleGetScreenshotRequest()
{
    assert(windowMgrCtrl != nullptr && "IRDKWindowManager interface is not initialized.");
    Core::hresult result = windowMgrCtrl->GetScreenshot();
    if (result == Core::ERROR_NONE)
    {
        std::cout << "Screenshot call placed successfully." << std::endl;
    }
    else
    {
        std::cout << "Failed to capture screenshot. Error code: " << result << std::endl;
    }
}