#ifndef YOURINPUTARGSPARSER
#define YOURINPUTARGSPARSER

#include "YourInputArgs.hh"

class YourInputArgParser {
public:
    YourInputArgParser(int argc, char** argv);

    bool parse();
    const YourInputArgs& args() const;

    static void help(const char* prog);

private:
    int argc_;
    char** argv_;
    YourInputArgs args_;

    bool parse_one(int& i);
};

#endif
