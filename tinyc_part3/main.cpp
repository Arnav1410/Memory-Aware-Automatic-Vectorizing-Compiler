#include "translator.hpp"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace tinyc3;

extern int yyparse(void);
extern FILE *yyin;
extern int yylineno;

int main(int argc, char **argv) {
    if (argc < 2 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " input.c [target.conf] [tac.out]\n";
        return 1;
    }

    try {
        TargetInfo target;
        if (argc >= 3) target = TargetInfo::fromConfigFile(argv[2]);
        else target = TargetInfo::hostAbi();

        Translator translator(target);
        gTranslator = &translator;

        yyin = std::fopen(argv[1], "r");
        if (!yyin) {
            std::perror(argv[1]);
            return 1;
        }

        int rc = yyparse();
        std::fclose(yyin);
        if (rc != 0) return 1;

        translator.print(std::cout);
        if (argc == 4) {
            std::ofstream out(argv[3]);
            if (!out) {
                std::cerr << "Cannot open " << argv[3] << "\n";
                return 1;
            }
            translator.printTac(out);
        }
    } catch (const std::exception& e) {
        std::cerr << "Translation error: " << e.what() << " near line " << yylineno << "\n";
        return 1;
    }
    return 0;
}
