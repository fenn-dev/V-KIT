#include "../VTokens.hpp"

enum class V_IR : uint64_t{
    NOTHING = 0,

    VARDECL = 1 << 0, ////----/////
    // Nothing = AUTO
    _CONST  = 1 << 1,
    _INT    = 1 << 2,
    _UINT   = 1 << 3,
    _WILD   = 1 << 4,
    _S8     = 1 << 5,
    _S16    = 1 << 6,
    _S32    = 1 << 7,
    _FLOAT  = 1 << 8,
    _DOUBLE = 1 << 9,
    _STRING = 1 << 10,
    _CHAR   = 1 << 11,

    MATH    = 1 << 1,
    // Nothing = EQUAL
    _ADD    = 1 << 2,
    _SUB    = 1 << 3,
    _MUL    = 1 << 4,
    _DIV    = 1 << 5,
    _LSH    = 1 << 6,
    _RSH    = 1 << 7,

    MISC    = 1 << 2,
    // Nothing INSERT
    _IMPORT = 1 << 3,
    _LINK   = 1 << 4
};

uint64_t operator||(V_IR arg1, V_IR arg2) {
    return static_cast<uint64_t>(arg1) << static_cast<uint64_t>(arg2);
}

struct node {
    uint64_t op;
    std::string p1;
    std::string p2;
};

class builder {
    public:
    std::vector<node> build(std::vector<Token> tokens);
    private:
    std::vector<node> nodes;
};