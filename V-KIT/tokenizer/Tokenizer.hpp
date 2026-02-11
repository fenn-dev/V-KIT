#include "../VTokens.hpp"
#include <string>

struct Token {
    std::string lexeme;
    size_t row;
    size_t column;
    TokenType type;

    Token(std::string l, size_t r, size_t c, TokenType t) 
        : lexeme(l), row(r), column(c), type(t) {}
};

class Tokenizer {
    public:
    std::vector<Token>& Tokenize(std::string_view src);
    [[nodiscard]] inline std::vector<Token>& retrieveTokens() { return tokens; }
    private:
    std::vector<Token> tokens;
}; 
