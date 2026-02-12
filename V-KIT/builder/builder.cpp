#include "builder.hpp"
#include "../Helper.hpp"

using std::vector;
using std::string;

vector<node> builder::build(vector<Token> tokens) {
    node currentNode;
    for (Token token : tokens) {
        DLOG("Builder Started");
        switch (token.type)
        {
        case TokenType::LET: {
            currentNode.op = static_cast<uint64_t>(V_IR::VARDECL);
            break;
        }
        case TokenType::CONST: {
            currentNode.op = V_IR::VARDECL || V_IR::_CONST;
        }
        case TokenType::IDENTIFIER: {
            currentNode.p1 = token.lexeme;
        }
            
        default:
            break;
        }
    }
}