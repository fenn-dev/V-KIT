#include "Tokenizer.hpp"
#include <stdexcept>
using std::string;
using std::string_view;
using std::vector;

TokenType Categorize(const string& buffer) {
    // 1. Check if it's a known keyword (let, i32, commit, etc)
    if (Keywords.contains(buffer)) {
        return Keywords.at(buffer);
    }

    // 2. Check if it's a Numeric Literal
    if (isdigit(buffer[0])) {
        return TokenType::INT_LITERAL;
    }

    // 3. If it starts with a letter or underscore, it's an IDENTIFIER
    if (isalpha(buffer[0]) || buffer[0] == '_') {
        return TokenType::IDENTIFIER;
    }

    // 4. Fallback for unknown symbols or errors
    throw std::runtime_error("Unknown token: " + buffer);
}

vector<Token>& Tokenizer::Tokenize(string_view src) {
    tokens.clear();
    string buffer = "";
    size_t currentRow = 0;
    size_t currentCol = 0;
    for (size_t i = 0; i < src.length(); ++i) {
        char c = src[i];
        currentCol++;

        if (c == '\"') {
            string literal = "";
            size_t startCol = currentCol;
            i++; 
            while (i < src.length() && src[i] != '\"') {
                if (src[i] == '\n') { currentRow++; currentCol = 0; }
                literal += src[i];
                i++; currentCol++;
            }
            if (i >= src.length()) throw std::runtime_error("Missing closing quote");
            tokens.emplace_back(literal, currentRow, startCol, TokenType::STRING_LITERAL);
            continue;
        }

        // 2. Structural Breaks (The "Categorize" Trigger)
        if (isspace(c) || c == ':' || c == '=' || c == '&' || c == '<' || c == ';') {
            if (!buffer.empty()) {
                // Use Categorize to distinguish between Keyword, Identifier, and Literal
                TokenType type = Categorize(buffer); 
                tokens.emplace_back(buffer, currentRow, currentCol - buffer.length() - 1, type);
                buffer = "";
            }

            if (isspace(c)) {
                if (c == '\n') { currentRow++; currentCol = 0; }
                continue;
            }

            // 3. Multi-character Move Operator (<-)
            string sym(1, c);
            if (c == '<' && i + 1 < src.length() && src[i + 1] == '-') {
                sym = "<-";
                i++; currentCol++;
            }

            if (Keywords.contains(sym)) {
                tokens.emplace_back(sym, currentRow, currentCol, Keywords.at(sym));
            }
            continue;
        }

        // 2. Word/Keyword Detection
        if (isspace(c) || c == ':' || c == '=' || c == '&' || c == '<' || c == ';') {
            if (!buffer.empty()) {
                TokenType type = Keywords.contains(buffer) ? Keywords.at(buffer) : TokenType::IDENTIFIER;
                tokens.emplace_back(buffer, currentRow, currentCol - buffer.length() - 1, type);
                buffer = "";
            }

            if (isspace(c)) {
                if (c == '\n') { currentRow++; currentCol = 0; }
                continue;
            }

            // 3. Handle Potential Multi-character operators (The Move: <-)
            string sym(1, c);
            if (c == '<' && i + 1 < src.length() && src[i + 1] == '-') {
                sym = "<-";
                i++; currentCol++;
            }

            if (Keywords.contains(sym)) {
                tokens.emplace_back(sym, currentRow, currentCol, Keywords.at(sym));
            }
            continue;
        }

        buffer += c;
    }
    return tokens;
}