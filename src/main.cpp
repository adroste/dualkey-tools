#include <iostream>
#include <memory>
#include <signal.h>
#include <vector>

#include "./lib/ConfigLoader.h"
#include "./lib/handler/HandlerComposer.h"
#include "./lib/handler/OutputKeyCodeHandler.h"
#include "./lib/hooks/LinuxKeyHook.h"

LinuxKeyHook keyHook;

void sigIntHandler(int)
{
    std::cout << "\nStopped by user..." << std::endl;
    keyHook.abort();
    exit(0);
}

void showUsage()
{
    std::cout
        << "Usage: dualkey-tools [OPTION]...\n"
        << "Options:\n"
        << "\t-h,--help\t\tShow this help message\n"
        << "\t-r,--rules FILE (yaml)\tSpecify the rules file. Defaults to rules.yaml\n"
        << "\t-t,--test\t\tTest mode. Useful for figuring out keycodes.\n"
        << "\nYou should run this tool with the highest process priority possible.\n"
        << ">> Unix: sudo nice -n -20 ./dualkey-tools"
        << std::endl;
}

int main(int argc, char* argv[])
{
    // TODO mutex / only allow one app instance
    // dev note: mutex is not needed in linux
    // because app will close because input device
    // cannot be grabbed a second time
    signal(SIGINT, sigIntHandler);

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    auto rulesFile = "rules.yaml";
    bool testMode = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            showUsage();
            return 0;
        } else if ((arg == "-r") || (arg == "--rules")) {
            if (i + 1 < argc) {
                rulesFile = argv[++i];
            } else {
                std::cerr << "-r,--rules option requires one argument." << std::endl;
                showUsage();
                return 1;
            }
        } else if ((arg == "-t") || (arg == "--test")) {
            testMode = true;
        } else {
            std::cerr << "unknown option: " << arg << std::endl;
            showUsage();
            return 1;
        }
    }

    try {
        if (testMode) {
            std::cout << "TEST MODE - use CTRL + C to quit" << std::endl;
            OutputKeyCodeHandler testHandler;
            keyHook.hookSync(testHandler);
        } else {
            auto composed = ConfigLoader::loadRulesFile(rulesFile);
            keyHook.hookSync(composed);
        }
    } catch (const std::exception& e) {
        printf("%s\n", e.what());
        return 1;
    }

    return 0;
}
