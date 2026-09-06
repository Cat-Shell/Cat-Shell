#include <filesystem>
#include <iostream>
#include <string>
#include <cstdlib>

// Включаем Windows.h только если компилируем под Windows
#ifdef _WIN32
#include <Windows.h>
#endif

using namespace std;

// Функция для вывода списка файлов и папок в текущей директории
void ls() {
    for (const auto &entry : filesystem::directory_iterator(filesystem::current_path())) {
        cout << entry.path().filename().string() << "\n";
    }
}

// Функция для получения текущего пути и формирования приглашения
string get_prompt() {
    return "🐱 " + filesystem::current_path().string() + " > ";
}

// Функция для печати приветственного сообщения
void print_welcome() {
    cout << R"(
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.1

)";
}

// Функция для печати справки по командам
void print_help() {
    cout << "help - эта справка\n";
    cout << "clear - очистить экран\n";
    cout << "meow - мяукнуть\n";
    cout << "pwd - текущий путь\n";
    cout << "ls - список файлов и папок\n";
    cout << "exit - выйти\n";
}

int main() {
    // Настройка кодировки для Windows
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");
#endif

    print_welcome();

    string input;

    while (true) {
        cout << get_prompt();
        getline(cin, input);

        if (input == "exit") {
            break;
        }
        else if (input == "help") {
            print_help();
        }
        else if (input == "meow") {
            cout << "Мяу!\n";
        }
        else if (input == "clear") {
            cout << "\033[H\033[J";
        }
        else if (input == "pwd") {
            cout << filesystem::current_path().string() << "\n";
        }
        else if (input == "ls") {
            ls();
        }
        else if (!input.empty()) {
            cout << "Cat-Shell: команда не найдена: " << input << "\n";
        }
    }

    return 0;
}
