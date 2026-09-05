#include <filesystem>
#include <iostream>
#include <string>
#include <cstdlib>
#include <Windows.h>

// ! пространство имен std
using namespace std;

// ! Функция для вывода списка файлов и папок в текущей директории
void ls() {
    // ! Используем filesystem для итерации по текущей директории
    for (const auto &entry : filesystem::directory_iterator(filesystem::current_path())) {
        // ! Выводим имя файла или папки
        cout << entry.path().filename().string() << "\n";
    }
}

// ! Функция для получения текущего пути и формирования приглашения
string get_prompt() {
    return "🐱 " + filesystem::current_path().string() + " > ";
}

// ! Функция для печати приветственного сообщения
void print_welcome() {
    cout << R"(
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.1

)";
}

// ! Функция для печати справки по командам
void print_help() {
    cout << "help - эта справка\n";
    cout << "clear - очистить экран\n";
    cout << "meow - мяукнуть\n";
    cout << "pwd - текущий путь\n";
    cout << "ls - список файлов и папок\n";
    cout << "exit - выйти\n";
}

int main() {

    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");

    // ! Печатаем приветственное сообщение
    print_welcome();

    string input;

    while (true) {
        // ! Выводим приглашение с текущим путём
        cout << get_prompt();

        // ! Получаем команду пользователя
        getline(cin, input);

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
            cout << "Мяу!\n";
        }

        // ! Обрабатываем команду clear для очистки экрана
        else if (input == "clear") {
            cout << "\033[H\033[J";
        }

        // ! Обрабатываем команду pwd для вывода текущего пути
        else if (input == "pwd") {
            cout << filesystem::current_path() << "\n";
        }

        // !Обрабатываем команду ls для вывода списка файлов и папок
        else if (input == "ls") {
                ls();
        }

        // ! Проверяем неизвестные команды
        else {
            cout << "Команда не найдена. Напиши help.\n";
        }

    }

    return 0;
}

