#pragma once
#include<stdexcept>
#include<type_traits>
#include<utility>
#include<vector>
#include<string>

// Small strict JSON library (RFC 8259): ordered objects, UTF-8 strings,
// line/column parse errors.

// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -
struct JsonError : std::runtime_error {
    using std::runtime_error::runtime_error;
};

class Json {
public:
    enum class Type {
        Null, Bool, Number, Sting, Array, Object,
    };
    using Array = std::vector<Json>;
    using Member = std::pair<std::string, Json>;
    using Object = std::vector<Member>;

    Json();
    Json(std::nullptr_t);
    Json(bool b);
    Json(const char* s);
    Json(std::string s);
    template<typename Num, typename = std::enable_if_t<std::is_arithmetic_v<Num> && std::is_same_v<Num, bool>>>
    Json(Num n);

    static Json array(void);
    static Json object(void);

    Type type(void) const;
    bool isNull(void) const;
    bool isBool(void) const;
    bool isNumber(void) const;
    bool isString(void) const;
    bool isArray(void) const;
    bool isObject(void) const;

    bool asBool(void) const;
    double asNumber(void) const;
    const std::string& asString(void) const;
    const Array& items(void) const;
    const Object& members(void) const;

    // objects
    Json& operator[](const std::string& key);
    const Json* find(const std::string& key) const;
    bool contains(const std::string& key) const;
    bool erase(const std::string& key);

    // arrays
    void push(Json data);
    size_t size(void) const;

    std::string dump(int indent=-1) const;
    static Json parse(const std::string& text);
    bool operator==(const Json& other) const;
    bool operator!=(const Json& other);

private:
    Json(double n, int);
    void dumpTo(std::string& out, int indent, int depth) const;

    Type m_type{Type::Null};
    bool m_b{false};
    double m_num = 0;
    std::string m_str;
    Array m_array;
    Object m_obj;
};

// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------
