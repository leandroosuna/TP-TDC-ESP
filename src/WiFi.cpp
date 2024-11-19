#include "WiFi.h"

AsyncWebServer server(80);
IPAddress localIP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
const char* PARAM_LINE_NUMBER = "line-number";
const char* PARAM_MESSAGE = "test-param";
const char* PARAM_DIR = "dir";

const char* SSID = "TDC-BOT";
const char* PASSWORD = "12345678";

int plotValue;

bool changed;

void setupWiFi()
{
    
    WiFi.mode(WIFI_AP);

    debugln("Starting wifi hotspot");
    WiFi.softAPConfig(localIP, gateway, subnet);
    WiFi.softAP(SSID, PASSWORD);
    IPAddress IP = WiFi.softAPIP();
    debug("AP IP address: ");
    debugln(IP);

    if (!SPIFFS.begin()) {
        debugln("Failed to mount file system");
        return;
    }
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/script.js", "text/javascript");
    });
    
    server.on("/data", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        }
        
        request->send(200, "text/plain", String(plotValue));

        digitalWrite(LED_BUILTIN, LOW);

        
    });

    server.onNotFound(handleNotFound);
    
    
    // Start the server
    server.begin();
    debugln("HTTP server started");
    if (MDNS.begin("bot")) {  // "bot.local" is the url
        debugln("MDNS responder started");
        
    } else {
        debugln("Error setting up MDNS responder");
    }
}
int k, obj;
void handleNotFound(AsyncWebServerRequest *request) {
    String path = request->url();

    if (path.startsWith("/k/")) {
        String kValue = path.substring(3); 
        k = kValue.toInt();             
        Serial.printf("Received /k/ with value: %d\n", k);
        changed = true;

        request->send(200, "text/plain", "Received k: " + String(k));
    } 
    else if (path.startsWith("/obj/")) {
        String objValue = path.substring(5); 
        obj = objValue.toInt();          
        Serial.printf("Received /obj/ with value: %d\n", obj);
        changed = true;

        request->send(200, "text/plain", "Received obj: " + String(obj));
    } 
    else {
        request->send(404, "text/plain", "Not Found");
    }
}

bool anyChange()
{
    bool update = changed;
    if(update)
    {
        changed = false;
    }    
    
    return update;
}
int getK()
{
    return k;
}
int getObj()
{
    return obj;
}

void newPoints(int mm, int obj)
{
    plotValue = mm;
}