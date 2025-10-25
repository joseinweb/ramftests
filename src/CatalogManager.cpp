#include <thread>

#include "CatalogManager.hpp"
#include <json/json.h>

CatalogManager::CatalogManager() : catalogData("") {}

CatalogManager::~CatalogManager()
{
    curl_global_cleanup();
}

void CatalogManager::displayMenu()
{
    while (true)
    {
        std::cout << "------------------------------------------------------------" << std::endl;

        std::cout << "Catalog Manager Menu:" << std::endl;
        std::cout << "1. Retrieve Catalog" << std::endl;
        std::cout << "2. Retrieve Application details" << std::endl;
        std::cout << "0. Return to previous menu" << std::endl;

        int choice = retrieveInputFromUser<int>("Enter your choice: ", false, 0);
        std::cout << "------------------------------------------------------------" << std::endl;
        switch (choice)
        {
        case 1:
            retrieveCatalog();
            break;
        case 2:
            retrieveApplicationDetails();
            break;
        case 0:
            return;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }
    }
}

bool CatalogManager::initialize()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    return true;
}

bool CatalogManager::retrieveApplicationDetails()
{
    std::string appId = retrieveInputFromUser<std::string>("Enter Application ID: ", false, "");
    std::string version = retrieveInputFromUser<std::string>("Enter Application Version: ", false, "");
    if (retrieveCatalogFromServer(appId, version))
    {
        // We got the catalog. Now let us get the list of applications.
        //{"header":{"url":"https://dac.dev.fireboltconnect.com/bundles/com.rdk.app.wpebrowser_2.38/1.0.1/rpi4/1.0.0-b34e9a38a2675d4cd02cf89f7fc72874a4c99eb0-dbg","size":55110007,"name":"WPEBrowser_2.38","description":"WPEBrowser_2.38","type":"application/dac.native","category":"application","id":"com.rdk.app.wpebrowser_2.38","version":"1.0.1","icon":"https://dac.dev.fireboltconnect.com/icons/com.rdk.app.wpebrowser_2.38/1.0.1"},"requirements":{"platform":{"architecture":"arm","variant":"v7","os":"linux"},"hardware":{"ram":"512M","dmips":"2000","persistent":"60M","cache":"200M"},"features":[{"name":"rdk.api.awc","version":"2","required":false}]},"maintainer":{"code":"rdk","name":"RDK","address":"RDK Management, LLC","homepage":"https://rdkcentral.com","email":"support@rdkcentral.com"},"versions":[{"version":"1.0.1"}]}

        Json::CharReaderBuilder builder;
        Json::Value root;
        JSONCPP_STRING err;
        Json::FastWriter writer;
        const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        if (reader->parse(catalogData.c_str(), catalogData.c_str() + catalogData.size(), &root, &err))
        {
            Json::Value header = root["header"];
            Json::Value requirements = root["requirements"];
            Json::Value maintainer = root["maintainer"];
            Json::Value versions = root["versions"];

            std::cout << "[Application details]: " << writer.write(header) << std::endl;
            std::cout << "[Requirements]: " << writer.write(requirements) << std::endl;
            std::cout << "[Maintainer]: " << writer.write(maintainer) << std::endl;
            std::cout << "[Versions]: " << writer.write(versions) << std::endl;
            return true;
        }
    }
    return false;
}

void CatalogManager::fetchCatalog(const std::string &url, bool addCredentials)
{
    CURL *curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        if (addCredentials)
        {
            curl_easy_setopt(curl, CURLOPT_USERNAME, serverConfig.user.c_str());
            curl_easy_setopt(curl, CURLOPT_PASSWORD, serverConfig.userKey.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallbackCb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
}

size_t CatalogManager::writeCallback(void *contents, size_t size, size_t nmemb)
{
    size_t totalSize = size * nmemb;
    catalogData = "";
    catalogData.append(static_cast<char *>(contents), totalSize);
    isCatalogReady = true;
    catalogCondVar.notify_one();
    return totalSize;
}

size_t CatalogManager::writeCallbackCb(void *contents, size_t size, size_t nmemb, void *userp)
{
    CatalogManager *self = static_cast<CatalogManager *>(userp);
    return self->writeCallback(contents, size, nmemb);
}

bool CatalogManager::retrieveCatalog()
{

    if (configUrl.empty() || platformType.empty() || apiKey.empty())
    {

        configUrl = retrieveInputFromUser<std::string>("Enter Catalog Config URL: ", true, "https://dac.config.dev.fireboltconnect.com/configuration/cpe.json");
        platformType = retrieveInputFromUser<std::string>("Enter Platform Type:  ", true, "ah212");
        apiKey = retrieveInputFromUser<std::string>("Enter Platform Key:  ", true, "1.0.0-97dc2fa24dbc788206db90934f9a2773efccebc2-dbg");
    }
    if (initialize())
    {
        if (retrieveUserCredentials())
        {
            if (retrieveCatalogFromServer("", ""))
            {
                // We got the catalog. Now let us get the list of applications.
                printAppCatalog();
            }
        }
    }
    // Additional logic to perform catalog retrieval can be added here
    return true;
}
// https://dac.dev.fireboltconnect.com/apps?platformName=rpi4&firmwareVer=1.0.0-b34e9a38a2675d4cd02cf89f7fc72874a4c99eb0-dbg%27;echo
bool CatalogManager::retrieveCatalogFromServer(const std::string &appId, const std::string &version)
{

    catalogData.clear();
    isCatalogReady = false;

    configUrl = serverConfig.url;
    configUrl += "/apps";
    if (!appId.empty() && !version.empty())
        configUrl += "/" + appId + ":" + version;
    configUrl += "?platformName=" + platformType;
    configUrl += "&firmwareVer=" + apiKey;
    auto credWorker = [this](std::string uri)
    {
        fetchCatalog(uri, true);
    };
    std::thread worker(credWorker, configUrl);
    std::unique_lock<std::mutex> lock(catalogMutex);
    catalogCondVar.wait(lock, [this]
                        { return isCatalogReady; });
    std::cout << "Server response received for catalog request." << std::endl;
    worker.join();
    return true;
}
void CatalogManager::printAppCatalog()
{
    Json::CharReaderBuilder builder;
    Json::Value root;
    JSONCPP_STRING err;
    //{"applications":[{"name":"YouTube 2025","description":"DAC App for YouTube 2025","type":"application/dac.native","size":10000000,"category":"application","id":"com.rdk.app.cobalt2025","version":"1.0.1","icon":"https://dac.dev.fireboltconnect.com/icons/com.rdk.app.cobalt2025/1.0.1"},{"name":"YouTube 2025 (RPI4)","description":"YouTube 2025 (RPI4)","type":"application/dac.native","size":10000000,"category":"application","id":"com.rdk.app.cobalt25_rpi4","version":"1.0.1","icon":"https://dac.dev.fireboltconnect.com/icons/com.rdk.app.cobalt25_rpi4/1.0.1"},{"name":"WPEBrowser_2.38","description":"WPEBrowser_2.38","type":"application/dac.native","size":10000000,"category":"application","id":"com.rdk.app.wpebrowser_2.38","version":"1.0.1","icon":"https://dac.dev.fireboltconnect.com/icons/com.rdk.app.wpebrowser_2.38/1.0.1"}],"meta":{"resultSet":{"count":3,"limit":10,"offset":0,"total":3}}}
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(catalogData.c_str(), catalogData.c_str() + catalogData.size(), &root, &err))
    {
        Json::Value metaInfo = root["meta"];
        std::cout << "Total applications : " << metaInfo["resultSet"]["count"].asInt() << std::endl;
        Json::Value applications = root["applications"];
        if (applications.isArray())
        {
            // root is an array of application objects
            for (const auto &app : applications)
            {
                std::cout << "App Name: " << app.get("name", "N/A").asString() << std::endl;
                std::cout << "App Id: " << app.get("id", "N/A").asString() << std::endl;
                std::cout << "App Version: " << app.get("version", "N/A").asString() << std::endl;
                std::cout << "-----------------------------" << std::endl;
            }
        }
        else
            std::cout << " Missing application object " << catalogData << std::endl;
    }
    else
    {
        std::cerr << "Failed to parse JSON: " << err << std::endl
                  << "Data Received: [" << catalogData << "]" << std::endl;
    }
}

bool CatalogManager::retrieveUserCredentials()
{
    if (!serverConfig.url.empty())
    {
        std::cout << "Credentials already acquired" << std::endl;
        return true;
    }
    isCatalogReady = false;
    catalogData.clear();

    auto credWorker = [this](std::string uri)
    {
        fetchCatalog(uri);
    };
    std::thread worker(credWorker, configUrl);

    std::unique_lock<std::mutex> lock(catalogMutex);
    catalogCondVar.wait(lock, [this]
                        { return isCatalogReady; });
    std::cout << "Server response received for credentials." << std::endl;
    worker.join();

    // Got User credentials. Now need to populate the configuration.
    Json::CharReaderBuilder builder;
    Json::Value root;
    JSONCPP_STRING err;

    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (reader->parse(catalogData.c_str(), catalogData.c_str() + catalogData.size(), &root, &err))
    {
        Json::Value appstoreCatalog = root["appstore-catalog"];
        serverConfig.url = appstoreCatalog.get("url", "").asString();

        Json::Value authentication = appstoreCatalog["authentication"];
        if (authentication.isObject())
        {
            serverConfig.user = authentication.get("user", "").asString();
            serverConfig.userKey = authentication.get("password", "").asString();
        }
        else
        {
            std::cout << " Missing authentication object " << catalogData << std::endl;
        }
    }
    else
    {
        std::cerr << "Failed to parse JSON: " << err << std::endl
                  << "Data Received: [" << catalogData << "]" << std::endl;
        return false;
    }
    std::cout << "User credentials acquired successfully." << std::endl;
    return true;
}
