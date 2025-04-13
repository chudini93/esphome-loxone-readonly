#include "esp8266_udp.h"

namespace esphome {
  namespace esp8266_udp {

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

    void ESP8266UdpComponent::send_string_data(std::string data) {
      if (send_string_buffer_.size() >= send_buffer_length_) {
        ESP_LOGW(TAG, "Send buffer full. Dropping oldest data.");
        send_string_buffer_.pop();
      }

      send_string_buffer_.push(data);
      ESP_LOGD(TAG, "Queued string data for sending: %s", data.c_str());
    }

  }  // namespace esp8266_udp
}  // namespace esphome
