#include <iostream>
#include <string>
#include <cstdlib>

const std::string PROMPT = "🐱 > ";

void print_welcome() {
    std::cout << R"(
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.1

)";
}

void print_help() {
    std::cout << "help - эта справка\n";
    std::cout << "clear - очистить экран\n";
    std::cout << "meow - мяукнуть\n";
    std::cout << "exit - выйти\n";
}

int main() {
    print_welcome();

    std::string input;

    while (true) {
        std::cout << PROMPT;
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        } else if (input == "help") {
            print_help();
        } else if (input == "meow") {
            std::cout << "Мяу! 😸\n";
        } else if (input == "clear") {
            std::cout << "\033[H\033[J";
        } else if (!input.empty()) {
            std::cout << "Команда не найдена. Напиши help.\n";
        }
    }

    return 0;
}