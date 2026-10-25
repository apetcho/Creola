/*
Typed, layered configuration:
    built-in defaults < global file (config.json) < session overrides.
*/
#pragma once
#include<filesystem>
#include<functional>
#include<vector>
#include<string>
#include<map>

#include "json.hpp"

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -

struct Setting {
    enum Kind {Bool, Num, Positive, Int, String, Color, Enum };
    
    std::string key;
    Kind kind;
    Json def;
    std::string doc;
    bool nullable = false;  // null means "inherit" (e.g. solve.tolerance -> tolerance)
    double lo = 0, hi = 0;  // Int range / Number range 8lo == hi : unbounded)
    std::vector<std::string> choices; // Enum
};

const std::vector<Setting>& settingsSchema(void);
const Setting* findSetting(const std::string& key); // exact key only

// resolves legacy aliases (title -> plot.title.text, ...)
std::string canonicalKey(const std::string& key);


std::filesystem::path configDir(void);

class ConfigStore{
public:
    enum class Scope {Session, Global };

    ConfigStore();

    // layered value (may be null for inheriting keys)
    Json get(const std::string& key) const;
    double number(const std::string& key, double fallback) const;
    bool flag(const std::string& key, bool fallback=false) const;
    std::string string(const std::string& key, const std::string& fallback="") const;

    // First non-null value int the chain, e.g. tolerance({"solve.equation.tolerance", "solve.tolerance", "tolerance"})
    double tolerance(std::initializer_list<const char*> chain) const;

    // Validates against the schema; throws std::runtime_error with a user-readable message.
    void set(const std::string& key, const Json& value, Scope scope=Scope::Session);
    void reset(const std::string& key, Scope scope=Scope::Session);
    bool isOverridden(const std::string& key) const;
    std::vector<std::string> keys(void) const;

    // Persistence. Both return false and fill `error` instead of throwing; bad entries
    // are skipped and reported.
    bool loadGlobal(std::string* warnings=nullptr);
    bool saveGlobal(std::string* error=nullptr) const;
    std::filesystem::path globalPath(void) const;
    void setGlobalPath(std::filesystem::path path);

    Json sessionSettings(void) const;
    void restoreSessionSettings(const Json& json, std::string* warnings=nullptr);

    std::function<void(void)> onChange;

private:
    Json validaded(const Setting& setting, const Json& json) const;

    std::map<std::string, Json> m_global;
    std::map<std::string, Json> m_session;
    std::filesystem::path m_globalPath;
};


// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------