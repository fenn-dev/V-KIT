#include "../VTokens.hpp"
#include <string>

class Tokenizer {
    public:
    std::vector<Token>& Tokenize(std::string_view src);
    [[nodiscard]] inline std::vector<Token>& retrieveTokens() { return tokens; }
    private:
    std::vector<Token> tokens;
}; 
