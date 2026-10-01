#include "commands.h"
#include <iostream>
#include <string>
#include <vector>
#include <clocale>
#include <csignal>

using namespace std;

int main(int argc, char* argv[]) {
// какой то мусор но он важен вроде не трогайте пока
#ifndef _WIN32
    struct sigaction sa{};
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, nullptr);
#else
    std::signal(SIGINT, sigint_handler);
#endif

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


    // Основной цикл
    while (running) {

        // сбрасываем флаг Ctrl+C перед новым промптом
        g_interrupted = false;

        // получаем строку приглашения для пользователя
        string prompt = get_prompt();

        // выводит приглашение без задержки
        cout << prompt << flush;

        // считавает команду, которую ввел пользователь
        input = read_input(prompt);

        // если Ctrl+C прилетел во время ввода — не выполняем команду
        if (g_interrupted) {
            g_interrupted = false;
            continue;
        }

        // добавляет команду в историю, если оно не повторяет последнюю и если не пустая
        if (!input.empty() && (history.empty() || history.back() != input)) {
            history.push_back(input);
        }
        // выполняет команду и говорит, прордолжить ли или не
        running = execute_command(input, history);
    }

    return 0;
}
