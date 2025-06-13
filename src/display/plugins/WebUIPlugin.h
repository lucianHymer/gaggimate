#ifndef WEBUIPLUGIN_H
#define WEBUIPLUGIN_H

#define ELEGANTOTA_USE_ASYNC_WEBSERVER 1

#include <DNSServer.h>

#include "../core/Plugin.h"
#include "GitHubOTA.h"
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESPAsyncWebServer.h>

constexpr size_t UPDATE_CHECK_INTERVAL = 5 * 60 * 1000;
constexpr size_t CLEANUP_PERIOD = 30 * 1000;
constexpr size_t STATUS_PERIOD = 1000;
constexpr size_t DNS_PERIOD = 10;

const String LOCAL_URL = "http://4.4.4.1/";
const String RELEASE_URL = "https://github.com/jniebuhr/gaggimate/releases/";

// OTA upload temporary file paths
constexpr const char* TMP_DISPLAY_FW_PATH = "/tmp_display.bin";
constexpr const char* TMP_DISPLAY_FS_PATH = "/tmp_filesystem.bin";
constexpr const char* TMP_CONTROLLER_FW_PATH = "/tmp_controller.bin";

class WebUIPlugin : public Plugin {
  public:
    WebUIPlugin();
    void setup(Controller *controller, PluginManager *pluginManager) override;
    void loop() override;

  private:
    void start(bool apMode);

    void handleOTASettings(uint32_t clientId, JsonDocument &request);
    void handleOTAStart(uint32_t clientId, JsonDocument &request);
    void handleAutotuneStart(uint32_t clientId, JsonDocument &request);
    void handleSettings(AsyncWebServerRequest *request) const;
    void handleBLEScaleList(AsyncWebServerRequest *request);
    void handleBLEScaleScan(AsyncWebServerRequest *request);
    void handleBLEScaleConnect(AsyncWebServerRequest *request);
    void handleBLEScaleInfo(AsyncWebServerRequest *request);
    void handleOTAUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final, const String& type);
    void updateOTAStatus(const String &version);
    void updateOTAProgress(uint8_t phase, int progress);
    void sendAutotuneResult();

    GitHubOTA *ota = nullptr;
    AsyncWebServer server;
    AsyncWebSocket ws;
    Controller *controller = nullptr;
    PluginManager *pluginManager = nullptr;
    DNSServer *dnsServer = nullptr;

    long lastUpdateCheck = 0;
    long lastStatus = 0;
    long lastCleanup = 0;
    long lastDns = 0;
    bool updating = false;
    String updateComponent = "";
    
    // File upload tracking
    struct UploadState {
        size_t totalSize = 0;
        size_t currentSize = 0;
        File file;
        String type;
        bool valid = true;
    };
    UploadState uploadState;
};

#endif // WEBUIPLUGIN_H
