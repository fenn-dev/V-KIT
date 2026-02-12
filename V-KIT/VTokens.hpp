#include <string>
#include <unordered_map>

#define KEYWORD_LIST(V)                     \
    V(LET, "let")                           \
    V(CONST, "const")                       \
    /* ___TYPES___ */                       \
    /* __INT__ */                           \
    V(I8, "i8")                             \
    V(I16, "i16")                           \
    V(I32, "i32")                           \
    V(I64, "i64")                           \
    V(INT, "int")                           \
    /* __UINT__ */                          \
    V(U8, "u8")                             \
    V(U16, "u16")                           \
    V(U32, "u32")                           \
    V(U64, "u64")                           \
    V(UINT, "uint")                         \
    /* __FLOATING__ */                      \
    V(F32, "f32")                           \
    V(FLOAT, "float")                       \
    V(F64, "f64")                           \
    V(DOUBLE, "double")                     \
    /* __WILDCARDS__ */                     \
    V(W8, "w8")                             \
    V(W16, "w16")                           \
    V(W32, "w32")                           \
    V(W64, "w64")                           \
    V(Wild, "wild")                         \
    /* ___OPERATORS___ */                   \
    V(ASSIGN, "=")                          \
    V(INHERIT, ":")                         \
    V(BORROW, "&")                          \
    V(MOVE, "<-")                           \
    V(COMMIT, ";")                          \
    /* __STRINGS__ */                       \
    V(STRING, "string")                     \
    V(CHAR, "char")                         \
    /* ___LITERALS___ */                    \
    V(STRING_LITERAL, "string_literal")     \
    V(INT_LITERAL, "int_literal")           \
    V(UINT_LITERAL, "uint_literal")         \
    V(IDENTIFIER, "identifier")             \
    V(NIL, "null")

enum class TokenType {
    #define DEFINE_ENUM(type, str) type,
    KEYWORD_LIST(DEFINE_ENUM)
    #undef DEFINE_ENUM
};

static inline const std::unordered_map<std::string, TokenType> Keywords = {
    #define DEFINE_MAP(type, str) { str, TokenType::type },
    KEYWORD_LIST(DEFINE_MAP)
    #undef DEFINE_MAP
};

struct Token {
    std::string lexeme;
    size_t row;
    size_t column;
    TokenType type;

    Token(std::string l, size_t r, size_t c, TokenType t) 
        : lexeme(l), row(r), column(c), type(t) {}
};