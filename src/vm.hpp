// Values, bytecode compiler and virtual machine.
#pragma once
#include<functional>
#include<variant>
#include<memory>
#include<vector>
#include<string>
#include<map>

#include "config.hpp"
#include "expr.hpp"
#include "lang.hpp"
#include "matrix.hpp"
#include "session.hpp"


// ----------------------------------------------------------------------------
// begin::namespace::creola
// ----------------------------------------------------------------------------
namespace creola{
// -

struct Figure;
struct Value;
using List = std::vector<Value>;

struct Equation{
    Expr lhs;
    Expr rhs;
    std::string var;
};

struct Pair {
    Expr x;
    Expr y;
};

struct Vec2D {
    Expr x1;
    Expr y1;
    Expr x2;
    Expr y2;
};

struct Range {
    long long lo = 0;
    long long hi = 0;
};

struct System {
    std::vector<Expr> lhs;
    std::vector<Expr> rhs;
    std::vector<std::string> vars;
};

struct Named;

struct Value {
    using Data = std::variant<std::monostate, Expr, std::string, std::shared_ptr<List>,
                        std::shared_ptr<Equation>, std::shared_ptr<Matrix>, Pair, Vec2D, Range,
                        std::shared_ptr<System>, std::shared_ptr<Named>>;

    Data data;

    Value() = default;
    Value(Expr expr);
    Value(std::string s);
    Value(List xs);
    Value(Equation eq);
    Value(Matrix mat);
    Value(Pair pair);
    Value(Vec2D vec2d);
    Value(Range rg);
    Value(System sys);
    Value(Named named);

    template<typename T>
    const T* as(void) const;
    bool isNil(void) const;
    bool isExpr(void) const;
    bool isList(void) const;
    bool isString(void) const;
    bool isEquation(void) const;
    bool isMatrix(void) const;
    bool isSystem(void) const;
    const Expr& expr(void) const;
    const List& list(void) const;
    const Equation& equation(void) const;
    const Matrix& matrix(void) const;
    const System& system(void) const;

    std::string toString(void) const;
};

struct Named {
    std::string name;
    Value value;
};

// -
enum class Op : unsigned char {
    Const,
    Load,
    Store,
    Add,
    Sub,
    Mul,
    Div,
    Pow,
    Neg,
    Call,
    MakeList,
    Index,
    MakeRange,
    MakeNamed,
    Pop,
    DefFun
};

struct Instr{
    Op op;
    int a = 0;
    int b = 0;
};

struct FunDef;

struct Chunk{
    std::vector<Instr> code;
    std::vector<Value> consts;
    std::vector<std::string> names;
    std::vector<std::shared_ptr<FunDef>> funs;
};

struct FunDef {
    std::string name;
    std::vector<std::string> params;
    Chunk body;
};

Chunk compile(const std::vector<AstPtr>& program);

// name -> {syntax, description}
const std::map<std::string, std::pair<std::string, std::string>>& commandDocs(void);

// -
class VM {
public:
    using Args = std::vector<Value>;
    using Builtin = std::function<Value(VM&, Args&)>;

    VM();
    ~VM();

    // -
    Value eval(const std::string& src, const std::function<void(const Value&)>& each = {});
    Value run(const Chunk& chunk);
    bool isCallable(const std::string& name) const;
    std::vector<std::string> commandNames(void) const;
    double cfgNum(const std::string& key, double fallback) const;
    void applySettings(void);

    std::map<std::string, Value> globals;
    std::map<std::string, std::shared_ptr<FunDef>> funs;
    std::map<std::string, Builtin> builtins;
    std::map<std::string, Value> named;
    ConfigStore settings;
    History history;
    std::function<void(void)> settingsChanged;
    std::shared_ptr<Figure> fig;
    std::function<void(const Figure&)> showHook;

private:
    Value call(const std::string& name, Args& args);

    std::vector<std::map<std::string, Value>> m_frams;
    int m_depth = 0;
};


// ----------------------------------------------------------------------------
}// end::namespace::creola
// ----------------------------------------------------------------------------