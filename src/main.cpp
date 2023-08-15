#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>


wifi_sta_list_t wifi_sta_list;
tcpip_adapter_sta_list_t adapter_sta_list;

void List_Connected_Update()
{
    memset(&wifi_sta_list, 0, sizeof(wifi_sta_list));
    memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));

    esp_wifi_ap_get_sta_list(&wifi_sta_list);
    tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list);

    for (int i = 0; i < adapter_sta_list.num; i++) {

        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];

        Serial.print("station nr ");
        Serial.println(i);

        Serial.print("MAC: ");
        for(int i = 0; i< 6; i++){
        
        Serial.printf("%02X", station.mac[i]);  
        if(i<5)Serial.print(":");
        }

        Serial.print("\nIP: ");  
        Serial.println(IPAddress(station.ip.addr));    
    }
}

void WiFiEvent(WiFiEvent_t event)
{
  Serial.printf("[WiFi-event] event: %d\n", event);

    switch (event) {
        case ARDUINO_EVENT_WIFI_READY: 
            Serial.println("WiFi interface ready");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            Serial.println("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            Serial.println("WiFi access point started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            Serial.println("AP IPv6 is preferred");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            Serial.println("STA IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP6:
            Serial.println("Ethernet IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_START:
            Serial.println("Ethernet started");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            Serial.println("Ethernet stopped");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            Serial.println("Ethernet connected");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            Serial.println("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Serial.println("Obtained IP address");
            break;
        default: break;
    }
}

void Client_Connected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.print("MAC from event: ");
    for (int z =0; z<6 ; ++z)
    {
        Serial.printf("%02X",info.wifi_ap_staconnected.mac[z]);
        if(z<5)Serial.print(":");
    }
    Serial.println();   
}

void Client_IP(WiFiEvent_t event, WiFiEventInfo_t info)
{

    Serial.print("IP connected from event: ");
    Serial.println(IPAddress(info.wifi_ap_staipassigned.ip.addr));
    List_Connected_Update();
}

void Client_Disconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.print("MAC from event: ");
    for (int z =0; z<6 ; ++z)
    {
        Serial.printf("%02X",info.wifi_ap_stadisconnected.mac[z]);
        if(z<5)Serial.print(":");
    }
    Serial.println();
    List_Connected_Update();

}

void setup() {
    Serial.begin(9600);
    Serial.println();
    WiFi.onEvent(WiFiEvent);
    WiFi.onEvent(Client_Connected,WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STACONNECTED);
    WiFi.onEvent(Client_IP, WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED);
    WiFi.onEvent(Client_Disconnected,WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("MyESP32AP");
}

void loop() {
    Serial.println("-----------");
    delay(5000);
}

