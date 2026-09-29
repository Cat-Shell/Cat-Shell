#include "commands.h"
#include <iostream>
#include <string>
#include <vector>
#include <clocale>

using namespace std;

int main() {
#ifdef _WIN32
    init_win_console();
    setlocale(LC_ALL, ".UTF8");
#endif

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--help" || arg == "-h") {
            cmd_help();
            return 0;
        }
    }

    print_welcome();

    string input;
    bool running = true;
    vector<string> history;

    while (running) {
        
        string prompt = get_prompt();

        cout << prompt << flush;

        input = read_input(prompt);

        if (!input.empty() && (history.empty() || history.back() != input)) {
            history.push_back(input);
        }
        running = execute_command(input, history);
    }

    return 0;
}
