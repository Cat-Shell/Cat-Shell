#include <filesystem>
#include <iostream>
#include <string>
#include <cstdlib>

// ! для краткого написания кода используем пространство имен std
using namespace std;


// ! Функция для получения текущего пути и формирования приглашения
std::string get_prompt() {
    return "🐱 " +  std::filesystem::current_path().string() + " > ";
}

// ! Функция для печати приветственного сообщения 
void print_welcome() {
    std::cout << R"(
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2

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
        std::cout << get_prompt();
        std::getline(std::cin, input);

        // ! Обработка команд
        if (input == "exit") {
            break;
        } 
        // ! Обработка команды help для вывода справки
        else if (input == "help") {
            print_help();
        } 
        // ! Обработка команды meow для мяуканья
        else if (input == "meow") {
            std::cout << "Мяу! 😸\n";
        } 
        // ! Обработка команды clear для очистки экрана
        else if (input == "clear") {
            std::cout << "\033[H\033[J";
        }
        // ! Обработка команды pwd для вывода текущего пути
        else if (input == "pwd") {
            std::cout << std::filesystem::current_path() << "\n";
        }
        // ! Обработка неизвестных команд
        else if (!input.empty()) {
            std::cout << "Команда не найдена. Напиши help.\n";
        }
    }

    return 0;
}