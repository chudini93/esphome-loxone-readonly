#pragma once

#include <string>
#include <queue>

#include "esphome.h"
#include "esphome/core/component.h"

#if defined(ESP8266)
  #include <WiFiUdp.h>
#else
  #error "This component is intended for ESP8266 only."
#endif

#define TAG "esp8266"

namespace esphome {
  namespace esp8266 {

    class OnStringDataTrigger;

    class ESP8266UdpComponent : public PollingComponent {
    public:
      ESP8266UdpComponent() : PollingComponent(5000) {}
      
      void setup() override;
      void update() override;
      void send_string_data(std::string data);

      void set_loxone_ip(std::string loxone_ip) {
        this->loxone_ip_ = loxone_ip;
      }
      void set_loxone_port(uint16_t port) {
        this->loxone_port_ = port;
      }
      void set_delimiter(std::string delimiter) {
        this->delimiter_ = delimiter;
      }
      void set_send_buffer_length(uint8_t buffer_length) {
        this->send_buffer_length_ = buffer_length;
      }

      void add_string_trigger(OnStringDataTrigger *trigger) {
        this->string_triggers_.push_back(trigger);
      }

    protected:
      std::vector<OnStringDataTrigger *> string_triggers_;
      std::string loxone_ip_;
      uint16_t loxone_port_;
      uint8_t send_buffer_length_;
      std::string delimiter_;
      std::string receive_string_buffer_;
      std::queue<std::string> send_string_buffer_;
      WiFiUDP udp_;
      bool udp_initialized_ = false;

      void ensure_connect_udp();
    };

    class OnStringDataTrigger : public Trigger<std::string>, public Component {
      friend class ESP8266UdpComponent;

    public:
      explicit OnStringDataTrigger(ESP8266UdpComponent *parent) : parent_(parent) {}

      void setup() override {
        this->parent_->add_string_trigger(this);
      }

    protected:
      ESP8266UdpComponent *parent_;
    };

  }  // namespace esp8266_udp
}  // namespace esphome
