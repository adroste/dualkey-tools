#include "./ConfigLoader.h"
#include "./CustomSymbol.h"
#include <iostream>

HandlerComposer ConfigLoader::loadRulesFile(const std::string& filename)
{
    YAML::Node conf;
    try {
        conf = YAML::LoadFile(filename);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: could not open rules file - " + filename);
    }

    HandlerComposer composed;

    try {
        if (!conf.IsSequence())
            throw new std::runtime_error("rules file must contain a list of rules");

        for (auto it = conf.begin(); it != conf.end(); ++it) {
            const auto& n = *it;

            if (!n["enabled"] || !n["type"] || !n["title"] || !n["options"])
                throw new std::runtime_error("each rule must contain properties: enabled, title, type and options");

            if (n["enabled"].as<std::string>() != "true")
                continue;

            std::cout << "Loading Rule: " << n["title"].as<std::string>() << std::endl;

            auto type = n["type"].as<std::string>();

            if (type == "dualkey")
                composed.keyHandler.push_back(dualKeyHandlerFromYaml(n["options"]));
            else if (type == "deadkey")
                composed.keyHandler.push_back(deadKeyHandlerFromYaml(n["options"]));
            else
                throw std::runtime_error("unknown rule type " + type);
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error: rules file invalid - ") + e.what());
    }

    return composed;
}

std::vector<ModifierFlags> ConfigLoader::getModifier(const YAML::Node& node)
{
    if (!node)
        throw std::runtime_error("modifier must be defined");

    if (!node.IsSequence())
        throw std::runtime_error("modifier must be a list of modifier flag lists. Example: [[Shift, CTRL], [None]]");

    std::vector<ModifierFlags> modifier;
    for (auto k = node.begin(); k != node.end(); ++k) {
        if (!k->IsSequence())
            throw std::runtime_error("modifier must be a list where each item itself is a list of modifier flags. Example: [[Shift, CTRL], [None]]");

        ModifierFlags f = ModifierFlags::NONE;
        for (auto i = k->begin(); i != k->end(); ++i) {
            std::string m;
            try {
                m = i->as<std::string>();
            } catch (const std::exception& e) {
                throw std::runtime_error("modifier must be a list of modifier flag lists. Example: [[Shift, Ctrl], [None]]");
            }
            if (m == "None")
                f |= ModifierFlags::NONE;
            else if (m == "Shift")
                f |= ModifierFlags::SHIFT;
            else if (m == "Ctrl")
                f |= ModifierFlags::CTRL;
            else if (m == "Alt")
                f |= ModifierFlags::ALT;
            else if (m == "AltGr")
                f |= ModifierFlags::ALT_GR;
            else if (m == "Win")
                f |= ModifierFlags::WIN;
            else
                throw std::runtime_error(m + " is a not supported modifier. Currently available: None, Shift, Ctrl, Alt, AltGr, Win");
        }
        modifier.push_back(f);
    }

    return modifier;
}

KeyCode ConfigLoader::getKey(const YAML::Node& parent, std::string key)
{
    const auto& n = parent[key];
    if (!n)
        throw std::runtime_error(key + " must be defined");

    if (!n.IsScalar())
        throw std::runtime_error(key + " must be single key code value");

    try {
        return static_cast<KeyCode>(n.as<int>());
    } catch (const std::exception& e) {
        throw std::runtime_error(key + " must be single key code value");
    }
}

std::vector<KeyCode> ConfigLoader::getKeys(const YAML::Node& parent, std::string key)
{
    const auto& n = parent[key];
    if (!n)
        throw std::runtime_error(key + " must be defined");

    if (!n.IsSequence())
        throw std::runtime_error(key + " must be a list of key code values. Example: [1, 2, 3]");

    std::vector<KeyCode> keys;
    for (auto k = n.begin(); k != n.end(); ++k) {
        if (!k->IsScalar())
            throw std::runtime_error(key + " must be a list of key code values. Example: [1, 2, 3]");
        try {
            keys.push_back(static_cast<KeyCode>(k->as<int>()));
        } catch (const std::exception& e) {
            throw std::runtime_error(key + " must be a list of key code values. Example: [1, 2, 3]");
        }
    }

    return keys;
}

std::vector<std::vector<KeyCode>> ConfigLoader::getKeySequence(const YAML::Node& parent, std::string key)
{
    const auto& n = parent[key];
    if (!n)
        throw std::runtime_error(key + " must be defined");

    if (!n.IsSequence())
        throw std::runtime_error(key + " must be a list of key code lists. Example: [[29, 1], [4], [3]]");

    std::vector<std::vector<KeyCode>> outer;
    for (auto k = n.begin(); k != n.end(); ++k) {
        if (!k->IsSequence())
            throw std::runtime_error(key + " must be a list where each element itself is also a list. Example: [[1], [2], [3]]");

        std::vector<KeyCode> inner;
        for (auto i = k->begin(); i != k->end(); ++i) {
            if (!i->IsScalar())
                throw std::runtime_error(key + " must be a list of lists of key code values. Example: [[1], [2,3], [4,5,6]]");
            try {
                inner.push_back(static_cast<KeyCode>(i->as<int>()));
            } catch (const std::exception& e) {
                throw std::runtime_error(key + " must be a list of lists of key code values. Example: [[1], [2,3], [4,5,6]]");
            }
        }
        outer.push_back(inner);
    }

    return outer;
}

std::unique_ptr<DualKeyHandler> ConfigLoader::dualKeyHandlerFromYaml(const YAML::Node& options)
{
    auto triggerKey = getKey(options, "triggerKey");
    auto tapKeySequence = getKeySequence(options, "tapKeySequence");
    auto holdKeys = getKeys(options, "holdKeys");
    auto ignoredKeys = getKeys(options, "ignoredKeys");

    return std::unique_ptr<DualKeyHandler>(new DualKeyHandler(triggerKey, tapKeySequence, holdKeys, ignoredKeys));
}

std::unique_ptr<DeadKeyHandler> ConfigLoader::deadKeyHandlerFromYaml(const YAML::Node& options)
{
    auto deadKey = getKey(options, "deadKey");
    const auto& symbolsNode = options["symbols"];

    if (!symbolsNode)
        throw std::runtime_error("symbols must be defined in deadkey options");

    if (!symbolsNode.IsSequence())
        throw std::runtime_error("symbols must be a list of dictionaries with properties: triggerKey, modifier and keySequence");

    std::vector<CustomSymbol> symbols;
    for (auto it = symbolsNode.begin(); it != symbolsNode.end(); ++it) {
        const auto& k = *it;
        if (!k.IsMap())
            throw std::runtime_error("symbols must be a list of dictionaries with properties: triggerKey, modifier and keySequence");

        auto triggerKey = getKey(k, "triggerKey");
        auto modifier = getModifier(k["modifier"]);
        auto keySequence = getKeySequence(k, "keySequence");
        symbols.push_back(CustomSymbol(triggerKey, modifier, keySequence));
    }

    return std::unique_ptr<DeadKeyHandler>(new DeadKeyHandler(deadKey, symbols));
}
