#include "builder.hpp"
#include "../Helper.hpp"
#include <stdexcept>

using std::vector;
using std::string;

void fillVar(vector<Token>& tokens, node& currentNode, int& i) {
    i++; 
    if (i >= tokens.size() || tokens[i].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected identifier after 'let' or 'const'");
    }

    currentNode.p1 = tokens[i].lexeme;

    while (++i < tokens.size()) {
        Token& token = tokens[i];

        if (token.type == TokenType::INHERIT) {
            i++;
            Token& typeToken = tokens[i];

            switch (typeToken.type) {
                case size8type:  currentNode.op << static_cast<uint64_t>(V_IR::_S8);  break;
                case size16type: currentNode.op << static_cast<uint64_t>(V_IR::_S16); break;
                case size32type: currentNode.op << static_cast<uint64_t>(V_IR::_S32); break;
                case sizeVARtype: 
                    if (arch == 64) currentNode.op << static_cast<uint64_t>(V_IR::_S64);
                    else currentNode.op << static_cast<uint64_t>(V_IR::_S32);
                    break;
            }

            switch (typeToken.type) {
                case INTtype:  currentNode.op << static_cast<uint64_t>(V_IR::_INT);  break;
                case UINTtype: currentNode.op << static_cast<uint64_t>(V_IR::_UINT); break;
                case WILDtype: currentNode.op << static_cast<uint64_t>(V_IR::_WILD); break;
            }
        }
        else if (token.type == TokenType::ASSIGN || token.type == TokenType::COMMIT) {
            i++;
            Token& typeToken = tokens[i];
            
            return; 
        }
    }
}

vector<node> builder::build(vector<Token> tokens) {
    node currentNode;
    for (int i = 0; i < tokens.size(); ++i) {
        Token token = tokens[i];
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
            
        default:
            break;
        }
    }
}