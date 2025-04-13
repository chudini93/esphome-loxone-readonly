#include "udp_esp8266_component.h"

namespace esphome {
  namespace udp_esp8266 {

    void ESP8266UdpComponent::setup() {}

    void ESP8266UdpComponent::update() {
      if (!network::is_connected()) {
        ESP_LOGD(TAG, "Network not ready");
        return;
      }
      
      // Handle outgoing buffer
      while (!send_string_buffer_.empty()) {
        udp_.beginPacket(loxone_ip_.c_str(), loxone_port_);
        udp_.write(send_string_buffer_.front().c_str());
        udp_.write(delimiter_.c_str());
        udp_.endPacket();

        ESP_LOGD(TAG, "Sent string data: %s", send_string_buffer_.front().c_str());
        send_string_buffer_.pop();
      }
    }

    bool disableUdpBuffer = true;
    void ESP8266UdpComponent::send_string_data(std::string data) {
      if (disableUdpBuffer){
        udp_.beginPacket(loxone_ip_.c_str(), loxone_port_);
        udp_.write(data);
        udp_.write(delimiter_.c_str());
        udp_.endPacket();
        
        ESP_LOGD(TAG, "Sent string data: %s", data);
        return;
      }
      
      if (send_string_buffer_.size() >= send_buffer_length_) {
        ESP_LOGW(TAG, "Send buffer full. Dropping oldest data.");
        send_string_buffer_.pop();
      }

      send_string_buffer_.push(data);
      ESP_LOGD(TAG, "Queued string data for sending: %s", data.c_str());
    }

  }  // namespace udp_esp8266
}  // namespace esphome
