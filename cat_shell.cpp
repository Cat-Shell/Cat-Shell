#include <filesystem>
#include <iostream>
#include <string>
#include <cstdlib>

std::string get_prompt() {
    return "🐱 " +  std::filesystem::current_path().string() + " > ";
}

void print_welcome() {
    std::cout << R"(
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.1

)";
}

void print_help() {
    std::cout << "help - эта справка\n";
    std::cout << "clear - очистить экран\n";
    std::cout << "meow - мяукнуть\n";
    std::cout << "pwd - текущий путь\n";
    std::cout << "exit - выйти\n";
}

int main() {
    print_welcome();

    std::string input;

    while (true) {
        std::cout << get_prompt();
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        } else if (input == "help") {
            print_help();
        } else if (input == "meow") {
            std::cout << "Мяу!\n";
        } else if (input == "clear") {
            std::cout << "\033[H\033[J";
        } else if (input == "pwd") {
            std::cout << std::filesystem::current_path() << "\n";
        } else if (!input.empty()) {
            std::cout << "Команда не найдена. Напиши help.\n";
        }
    }

    return 0;
}
