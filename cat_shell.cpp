#include <filesystem>
#include <iostream>
#include <string>
#include <cstdlib>

// ! Функция для получения текущего пути и формирования приглашения
std::string get_prompt() {
    return "🐱 " + std::filesystem::current_path().string() + " > ";
}

// ! Функция для печати приветственного сообщения
void print_welcome() {
    std::cout << R"(
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.1

)";
}

// ! Функция для печати справки по командам
void print_help() {
    std::cout << "help - эта справка\n";
    std::cout << "clear - очистить экран\n";
    std::cout << "meow - мяукнуть\n";
    std::cout << "pwd - текущий путь\n";
    std::cout << "exit - выйти\n";
}

int main() {
    // ! Печатаем приветственное сообщение
    print_welcome();

    std::string input;

    while (true) {
        // ! Выводим приглашение с текущим путём
        std::cout << get_prompt();

        // ! Получаем команду пользователя
        std::getline(std::cin, input);

        // ! Проверяем команду выхода из Cat-Shell
        if (input == "exit") {
            break;
        }

        // ! Обрабатываем команду help для вывода справки
        else if (input == "help") {
            print_help();
        }

        // ! Обрабатываем команду meow для вывода мяуканья
        else if (input == "meow") {
            std::cout << "Мяу!\n";
        }

        // ! Обрабатываем команду clear для очистки экрана
        else if (input == "clear") {
            std::cout << "\033[H\033[J";
        }

        // ! Обрабатываем команду pwd для вывода текущего пути
        else if (input == "pwd") {
            std::cout << std::filesystem::current_path() << "\n";
        }

        // ! Проверяем неизвестные команды
        else if (!input.empty()) {
            std::cout << "Команда не найдена. Напиши help.\n";
        }
    }

    return 0;
}
