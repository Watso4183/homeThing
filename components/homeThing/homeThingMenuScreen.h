#pragma once

#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include "esphome/components/homeThing/homeThingMenuTitle.h"
#include "esphome/components/homeThing/homeThingMenuTitleValue.h"
#include "esphome/core/entity_base.h"

#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif

#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif

#ifdef SHOW_VERSION
#include "version.h"
#endif

#ifdef USE_LIGHT
#include "esphome/components/light/light_state.h"
#endif

#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#endif

#ifdef USE_COVER
#include "esphome/components/cover/cover.h"
#endif

#ifdef USE_BUTTON
#include "esphome/components/button/button.h"
#endif

#ifdef USE_FAN
#include "esphome/components/fan/fan.h"
#endif

#ifdef USE_SELECT
#include "esphome/components/select/select.h"
#endif

namespace esphome {
namespace homething_menu_base {

class MenuCommand : public EntityBase {
 public:
  void on_command() { this->on_command_callbacks_.call(); }
  void add_on_command_callback(std::function<void()>&& callback) {
    this->on_command_callbacks_.add(std::move(callback));
  }

  std::string get_name() const { return name_; }

  void set_name(std::string val) { this->name_ = val; }

 private:
  CallbackManager<void()> on_command_callbacks_{};
  std::string name_;
};

enum MenuItemType {
  MenuItemTypeNone,
  MenuItemTypeTitle,
  MenuItemTypeSwitch,
  MenuItemTypeTextSensor,
  MenuItemTypeCommand,
  MenuItemTypeSensor,
  MenuItemTypeLight,
  MenuItemTypeNumber,
  MenuItemTypeCover,
  MenuItemTypeButton,
  MenuItemTypeFan,
  MenuItemTypeSelect
};

class HomeThingMenuScreen {
 public:
  explicit HomeThingMenuScreen(std::string name) : name_(name) {}
  void set_name(std::string name) { name_ = name; }
  std::string get_name() { return name_; }
  void set_index(int index) { index_ = index; }
  int get_index() { return index_; }
  void set_show_version(bool show_version) { show_version_ = show_version; }
  void set_selected_entity(
      const std::tuple<MenuItemType, EntityBase*, std::string>* entity) {
    selected_entity_ = entity;
  }
  const std::tuple<MenuItemType, EntityBase*, std::string>*
  get_selected_entity() {
    return selected_entity_;
  }
  int get_entity_count() { return entities_.size(); }
  std::string entity_name_at_index(int index);

#ifdef USE_SWITCH
  void register_switch(switch_::Switch* new_switch, std::string name) {
    entities_.push_back(std::make_tuple(MenuItemTypeSwitch, new_switch, name));
    new_switch->add_on_state_callback(
        [this, new_switch](bool state) { this->callback_.call(); });
  }
#endif

#ifdef USE_TEXT_SENSOR
  void register_text_sensor(text_sensor::TextSensor* new_text_sensor,
                            std::string name) {
    entities_.push_back(
        std::make_tuple(MenuItemTypeTextSensor, new_text_sensor, name));
    new_text_sensor->add_on_state_callback(
        [this, new_text_sensor](std::string state) { this->callback_.call(); });
  }
#endif

#ifdef USE_COMMAND
  void register_command(MenuCommand* new_command, std::string name) {
    entities_.push_back(
        std::make_tuple(MenuItemTypeCommand, new_command, name));
  }
#endif

#ifdef USE_SENSOR
  void register_sensor(sensor::Sensor* new_sensor, std::string name) {
    entities_.push_back(std::make_tuple(MenuItemTypeSensor, new_sensor, name));
    new_sensor->add_on_state_callback(
        [this, new_sensor](float state) { this->callback_.call(); });
  }
#endif

#ifdef USE_LIGHT
  void register_light(light::LightState* new_light, std::string name) {
    entities_.push_back(std::make_tuple(MenuItemTypeLight, new_light, name));
    new_light->add_new_remote_values_callback(
        [this, new_light]() { this->callback_.call(); });
  }
#endif

#ifdef USE_COVER
  void register_cover(cover::Cover* new_cover, std::string name) {
    entities_.push_back(std::make_tuple(MenuItemTypeCover, new_cover, name));
    new_cover->add_on_state_callback(
        [this, new_cover]() { this->callback_.call(); });
  }
#endif

#ifdef USE_NUMBER
  void register_number(number::Number* new_number, std::string name) {
    entities_.push_back(std::make_tuple(MenuItemTypeNumber, new_number, name));
    new_number->add_on_state_callback(
        [this, new_number](float state) { this->callback_.call(); });
  }
#endif

#ifdef USE_BUTTON
  void register_button(button::Button* new_button, std::string name) {
    entities_.push_back(std::make_tuple(MenuItemTypeButton, new_button, name));
    new_button->add_on_press_callback(
        [this, new_button]() { this->callback_.call(); });
  }
#endif

#ifdef USE_FAN
  void register_fan(fan::Fan* new_fan, std::string name) {
    entities_.push_back(std::make_tuple(MenuItemTypeFan, new_fan, name));
    new_fan->add_on_state_callback(
        [this, new_fan]() { this->callback_.call(); });
  }
#endif

#ifdef USE_SELECT
  void register_select(select::Select* new_select, std::string name) {
    entities_.push_back(std::make_tuple(MenuItemTypeSelect, new_select, name));
    new_select->add_on_state_callback(
        [this, new_select](std::string newValue, size_t num) {
          this->callback_.call();
        });
  }
#endif

  void add_on_state_callback(std::function<void()>&& callback) {
    this->callback_.add(std::move(callback));
  }
  void menu_titles(std::vector<MenuTitleBase*>* menu_titles, bool show_name);
  bool select_menu(int index);
  bool select_menu_hold(int index);
  const std::tuple<MenuItemType, EntityBase*, std::string>* get_menu_item(
      int index);

 private:
  bool show_name_ = false;
  int index_;
  bool show_version_ = false;
  std::string name_;
  std::vector<std::tuple<MenuItemType, EntityBase*, std::string>> entities_;
  const std::tuple<MenuItemType, EntityBase*, std::string>* selected_entity_ =
      nullptr;
  CallbackManager<void()> callback_;
  const char* const TAG = "homething.menu.screen";
};

}  // namespace homething_menu_base
}  // namespace esphome
