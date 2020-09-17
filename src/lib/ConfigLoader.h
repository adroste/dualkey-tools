#pragma once

#include "./ModifierFlags.h"
#include "./handler/DeadKeyHandler.h"
#include "./handler/DualKeyHandler.h"
#include "./handler/HandlerComposer.h"
#include <memory>
#include <yaml-cpp/yaml.h>

class ConfigLoader {
public:
    static HandlerComposer loadRulesFile(const std::string& filename);

private:
    static std::unique_ptr<DualKeyHandler> dualKeyHandlerFromYaml(const YAML::Node& options);
    static std::unique_ptr<DeadKeyHandler> deadKeyHandlerFromYaml(const YAML::Node& options);

    static std::vector<ModifierFlags> getModifier(const YAML::Node& node);
    static KeyCode getKey(const YAML::Node& parent, std::string key);
    static std::vector<KeyCode> getKeys(const YAML::Node& parent, std::string key);
    static std::vector<std::vector<KeyCode>> getKeySequence(const YAML::Node& parent, std::string key);
};
