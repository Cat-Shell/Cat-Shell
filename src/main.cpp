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

    print_welcome();

    string input;
    bool running = true;
    vector<string> history;

    while (running) {
        cout << get_prompt() << flush;

        if (!getline(cin, input)) {
            break;
        }

        history.push_back(input);
        running = execute_command(input, history);
    }

    return 0;
}
