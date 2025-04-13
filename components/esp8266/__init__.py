import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation

from esphome.const import (
    CONF_ID,
    CONF_TRIGGER_ID,
)

DEPENDENCIES = ['network']

my_ns = cg.esphome_ns.namespace('esp8266_udp')
ESP8266UdpComponent = my_ns.class_('ESP8266UdpComponent', cg.PollingComponent)
OnStringDataTrigger = my_ns.class_(
    "OnStringDataTrigger",
    automation.Trigger.template(cg.std_string, cg.Component)
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ESP8266UdpComponent),
    cv.Required("loxone_ip"): cv.ipv4address,
    cv.Required("loxone_port"): cv.int_range(0, 65535),
    cv.Optional("send_buffer_length", default=64): cv.int_range(0, 1024),
    cv.Optional("delimiter", default="\n"): cv.string,
    cv.Optional("on_string_data"): automation.validate_automation({
        cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(OnStringDataTrigger),
    }),
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_loxone_ip(str(config["loxone_ip"])))
    cg.add(var.set_loxone_port(config["loxone_port"]))
    cg.add(var.set_send_buffer_length(config["send_buffer_length"]))
    cg.add(var.set_delimiter(config["delimiter"]))
    yield cg.register_component(var, config)

    for conf in config.get("on_string_data", []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        cg.add(var.add_string_trigger(trigger))
        yield automation.build_automation(trigger, [(cg.std_string, "data")], conf)
