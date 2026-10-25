/*
Command history and per-session state that is saved in the background.
*/
#pragma once

#include<condition_variable>
#include<filesystem>
#include<optional>
#include<string>
#include<vector>
#include<atomic>
#include<thread>
#include<mutex>

#include "json.hpp"


// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
class History{
public:
    explicit History(size_t maxEntries=1000);
    void setMax(size_t n);

    bool add(const std::string& entry);
    const std::vector<std::string>& entries(void);
    size_t size(void) const;
    void clear(void);

    // -
    std::optional<std::string> previous(const std::string& current);
    std::optional<std::string> next(void);
    void resetCursor(void);

    Json toJson(void) const;
    void fromJson(const Json& array);
    bool load(const std::filesystem::path& file, std::string* error=nullptr);
    bool save(const std::filesystem::path& file, std::string* error=nullptr) const;

private:
    void trim(void);

    std::vector<std::string> m_items;
    size_t m_max;
    size_t m_cursor = 0;
    std::string m_draft;
};

// -
class Session {
public:
    explicit Session(std::filesystem::path dir={}, int autosaveMs=1000);
    ~Session();
    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    const std::string& id(void) const;
    std::filesystem::path file(void) const;
    void start(void);
    void flush(void);
    std::string lastError(void) const;

    void setSettings(Json overrides);
    void setHistory(Json entries);
    void setNotebook(std::string text);

    // -
    static std::optional<Json> loadLatest(const std::filesystem::path& dir, const std::string& excludedId={});
    static void prune(const std::filesystem::path& dir, size_t keep);

private:
    void touch(void);
    void writeSnapshot(void);
    void worker(void);

    std::filesystem::path m_dir;
    std::string m_id;
    std::string m_started;
    mutable std::mutex m_mtx;
    std::condition_variable m_cv;
    std::thread m_thread;
    Json m_settings = Json::object();
    Json m_history = Json::array();
    std::string m_notebook;
    std::string m_error;
    bool m_dirty = false;
    bool m_stop = false;
    bool m_running = false;    
};


// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------