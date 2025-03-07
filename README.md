# Loxone Component
more information, you can check with KinCony's webpage: https://www.kincony.com

# Core Yaml
```yaml
external_components:
- source: github://chudini93/esphome-loxone-readonly@v2.0.0
  components: [loxone]

loxone:
  id: loxone_udp
  loxone_ip: "192.168.50.124" # loxone server ip
  loxone_port: 9999 # loxone server port
  listen_port: 8888

binary_sensor:
  # IN1
  - platform: gpio
    id: di1
    name: di1
    pin:
      pcf8574: pcf_inputs
      number: 1
      mode:
        input: true
      inverted: true
    publish_initial_state: true
    on_state:
      - lambda: !lambda |-
          id(loxone_udp).send_string_data("di1=" + to_string(int(id(di1).state)));

light:
  # chl_01
  - platform: monochromatic
    output: ledc_chl01
    id: chl_01
    name: chl_01
    default_transition_length: ${light_default_transition_time}
    flash_transition_length: 5s
    gamma_correct: 0
    restore_mode: RESTORE_DEFAULT_OFF
    effects:
      - pulse:
      - pulse:
          name: "FastPulse"
          transition_length: 0.5s
          update_interval: 0.5s
          min_brightness: 0%
          max_brightness: 100%
      - pulse:
          name: "SlowPulse"
          transition_length: 1s # defaults to 1s
          update_interval: 2s
      - pulse:
          name: "AsymmetricalPulse"
          transition_length:
            on_length: 1s
            off_length: 500ms
          update_interval: 1.5s
    on_state:
      - lambda: !lambda |-
          auto values = id(chl_01).remote_values;
          id(loxone_udp).send_string_data("chl_01;state=" + to_string(int(values.get_state())) + ";brightness=" + to_string(int(values.get_brightness() * 255)));
```
