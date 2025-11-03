#pragma once
#include "common.hpp"
#include "WPEFramework/interfaces/IAppPackageManager.h"
#include "MgrControl.hpp"

inline std::string mapFailureReason(Exchange::IPackageInstaller::FailReason &type)
{
    switch (type)
    {
    case Exchange::IPackageInstaller::FailReason::SIGNATURE_VERIFICATION_FAILURE:
        return "SIGNATURE_VERIFICATION_FAILURE";
    case Exchange::IPackageInstaller::FailReason::PACKAGE_MISMATCH_FAILURE:
        return "PACKAGE_MISMATCH_FAILURE";
    case Exchange::IPackageInstaller::FailReason::INVALID_METADATA_FAILURE:
        return "INVALID_METADATA_FAILURE";
    case Exchange::IPackageInstaller::FailReason::PERSISTENCE_FAILURE:
        return "PERSISTENCE_FAILURE";
    default:
        return "NONE";
    }
}
inline std::string to_string(Exchange::IPackageInstaller::InstallState state)
{
    switch (state)
    {
    case Exchange::IPackageInstaller::InstallState::INSTALLING:
        return "INSTALLING";
    case Exchange::IPackageInstaller::InstallState::INSTALLATION_BLOCKED:
        return "INSTALLATION_BLOCKED";
    case Exchange::IPackageInstaller::InstallState::INSTALLED:
        return "INSTALLED";
    case Exchange::IPackageInstaller::InstallState::INSTALL_FAILURE:
        return "INSTALL_FAILURE";
    case Exchange::IPackageInstaller::InstallState::UNINSTALLING:
        return "UNINSTALLING";
    case Exchange::IPackageInstaller::InstallState::UNINSTALL_FAILURE:
        return "UNINSTALL_FAILURE";
    case Exchange::IPackageInstaller::InstallState::UNINSTALLED:
        return "UNINSTALLED";
    default:
        return "UNKNOWN";
    }
}

class PkgInstallEvtHandler : public Exchange::IPackageInstaller::INotification
{
public:
    ~PkgInstallEvtHandler() {}
    void OnAppInstallationStatus(const string &jsonresponse)
    {
        cout << "Installation Status: " << jsonresponse << endl;
    }
    BEGIN_INTERFACE_MAP(PkgInstallEvtHandler)
    INTERFACE_ENTRY(Exchange::IPackageInstaller::INotification)
    END_INTERFACE_MAP
};

class InstallMgrCtrl : public MgrCtrl
{
private:
    Exchange::IPackageInstaller *instlCtl;
    shared_ptr<Exchange::IPackageInstaller::INotification> instlEventHandler = nullptr;

    void handleStartInstallRequest();
    void handleUninstallRequest();
    void handleListPackagesRequest();
    void handlePackageInstallStateRequest();
    void handlePackageMetadataRequest();
    void handlePackageConfigurationRequest();

public:
    InstallMgrCtrl();
    ~InstallMgrCtrl();
    bool initialize(Core::ProxyType<RPC::CommunicatorClient> &client) override;
    bool checkPluginStatus() override;
    void displayMenu() override;
};
