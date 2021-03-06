#include "esphomelib/defines.h"

#ifdef USE_HOMEASSISTANT_SENSOR

#include "esphomelib/sensor/homeassistant_sensor.h"
#include "esphomelib/api/api_server.h"
#include "esphomelib/log.h"

ESPHOMELIB_NAMESPACE_BEGIN

namespace sensor {

static const char *TAG = "sensor.homeassistant";

HomeassistantSensor::HomeassistantSensor(const std::string &name, const std::string &entity_id)
    : Sensor(name), entity_id_(entity_id) {

}
void HomeassistantSensor::setup() {
  api::global_api_server->subscribe_home_assistant_state(this->entity_id_, [this](std::string state) {
    char *end;
    float value = ::strtof(state.c_str(), &end);
    if (end == nullptr) {
      ESP_LOGW(TAG, "Can't convert '%s' to number!", state.c_str());
      this->publish_state(NAN);
      return;
    }

    ESP_LOGD(TAG, "'%s': Got state %.2f", this->entity_id_.c_str(), value);
    this->publish_state(value);
  });
}
void HomeassistantSensor::dump_config() {
  LOG_SENSOR("", "Homeassistant Sensor", this);
  ESP_LOGCONFIG(TAG, "  Entity ID: '%s'", this->entity_id_.c_str());
}
float HomeassistantSensor::get_setup_priority() const {
  return setup_priority::WIFI;
}

} // namespace sensor

ESPHOMELIB_NAMESPACE_END

#endif //USE_HOMEASSISTANT_SENSOR
