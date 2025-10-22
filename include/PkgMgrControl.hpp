#pragma once
#include "common.hpp"
#include "WPEFramework/interfaces/IPackageManager.h"
#include "MgrControl.hpp"


class PackageManagerEventHandler : public Exchange::IPackageManager::INotification
{
public:
    ~PackageManagerEventHandler() {}
    void OperationStatus(const string& handle, const string& operation, const string& type, const string& id,
                         const string& version, const string& status, const string& details)
    {
        std::cout << "Operation Status:\n";
        std::cout << "  Handle: " << handle << "\n";
        std::cout << "  Operation: " << operation << "\n";
        std::cout << "  Type: " << type << "\n";
        std::cout << "  ID: " << id << "\n";
        std::cout << "  Version: " << version << "\n";
        std::cout << "  Status: " << status << "\n";
        std::cout << "  Details: " << details << "\n";
    }  
    uint32_t AddRef() const
    {
        cout << " Hey I (PackageManagerEventHandler::AddRef) am getting called  " << endl;
        return Core::ERROR_NONE;
    }
    uint32_t Release() const
    {
        cout << " Hey I (PackageManagerEventHandler::Release) am getting called " << endl;
        return Core::ERROR_NONE;
    }
    void *QueryInterface(const uint32_t interfaceNumber)
    {
        cout << " Hey I (PackageManagerEventHandler::QueryInterface) am getting called " << endl;
        if (interfaceNumber == Exchange::IPackageManager::INotification::ID)
        {
            return static_cast<Exchange::IPackageManager::INotification *>(this);
        }
        return nullptr;
    }
};

class PkgMgrControl: public MgrCtrl
{
private:
    Exchange::IPackageManager *pkgManager;
    shared_ptr<Exchange::IPackageManager::INotification> pkgMgrEvtHandler = nullptr;

public:
    PkgMgrControl(/* args */);
    ~PkgMgrControl();
    bool initialize(Core::ProxyType<RPC::CommunicatorClient> &client) override;
    void checkPluginStatus() override;
    void displayMenu() override;
};