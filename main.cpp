#define REMBLOAT

#include "V-KIT/tokenizer/Tokenizer.hpp"
#include <iostream>
#include <vector>
#include <string>
#ifndef REMBLOAT
#include "Visual.hpp"
#endif

int main(int argc, char** argv) {
    Tokenizer app;
    
    std::string src = R"(let secret: i32 = 42;
let secret2 = 3;
let resource: w64 <- secret2;

const message: string = "Vulpine is alive";

let copy: i32 = secret;
)";

    bool visual = false;
#ifndef REMBLOAT
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--visual") {
            visual = true;
        }
    }
#endif

    if (visual) {
#ifndef REMBLOAT
    RunVisualMode(app, src);
#endif    
    } else {
        std::cout << "--- Vulpine Console Tokenizer ---" << std::endl;
        for (auto const& token : app.Tokenize(src)) {
            std::cout << "[" << token.lexeme << "] TypeID: " << (int)token.type << std::endl;
        }
    }

    return 0;
}