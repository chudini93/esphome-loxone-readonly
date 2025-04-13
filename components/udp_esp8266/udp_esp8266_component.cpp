#include "udp_esp8266_component.h"

namespace esphome {
  namespace udp_esp8266 {

    void ESP8266UdpComponent::setup() {}

    void ESP8266UdpComponent::update() {}

    void ESP8266UdpComponent::send_string_data(std::string data) {
        udp_.beginPacket(loxone_ip_.c_str(), loxone_port_);
        udp_.write(data.c_str());
        udp_.write(delimiter_.c_str());
        udp_.endPacket();
        
        ESP_LOGD(TAG, "UDP message sent: %s", data.c_str());
    }

  }  // namespace udp_esp8266
}  // namespace esphome
