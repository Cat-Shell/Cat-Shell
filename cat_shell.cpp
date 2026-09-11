#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace std;

// ! Анимация ожидания: печатает msg, три точки с задержкой и перевод строки
void wait_dots(const string &msg) {
    cout << msg;
    for (int i = 0; i < 3; i++) {
        cout << ".";
        cout.flush();  // ! Принудительно выводим точку до задержки
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    cout << "\n";
}

// ! Печать приветственного сообщения
void print_welcome() {
    cout << R"(
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.3

)";
    cout << "Котик ждет твоей команды.\n\n";
}

// ! Вывод списка файлов и папок
void cmd_ls() {

    wait_dots("Котенок перебирает файлы и папки в текущей папке, просим помяукать и подождать");

    for (const auto &entry : filesystem::directory_iterator(filesystem::current_path())) {
        cout << entry.path().filename().string() << "\n";
    }

}

// ! Печать текущего пути
void cmd_pwd() {
    cout << filesystem::current_path().string() << "\n";
}

// ! Очистка экрана
void cmd_clear() {

    wait_dots("Котик уже бежит все слизывать");

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// ! Мяуканье
void cmd_meow() {
    cout << "Мяу!\n";
}

// ! Печать справки по командам
void cmd_help() {

    wait_dots("Котик торопиться достать листок с подсказками");

    cout << R"(
Доступные команды:
  help           - показать эту справку
  clear          - очистить экран
  meow           - мяукнуть
  pwd            - показать текущий путь
  ls             - список файлов и папок
  exit           - выйти из оболочки
  
Примеры:
  ls
  meow
)";
}

// ! Обработка и выполнение команд
bool execute_command(const string &input) {
    if (input.empty()) {
        return true; // ! Пропуск пустой строки
    } if (input == "exit") {
        cout << "Котик будет по тебе скучать(" << endl;
        return false; // ! Сигнал для выхода из цикла
    }

    if (input == "help") {
        cmd_help();
    } else if (input == "clear") {
        cmd_clear();
    } else if (input == "meow") {
        cmd_meow();
    } else if (input == "pwd") {
        cmd_pwd();
    } else if (input == "ls") {
        cmd_ls();
    } else {
        cout << "Cat-Shell: команда не найдена: " << input << "\n";
        cout << "Котик не нашел ее" << endl;
    }

    return true;
}

// ! Формирование строки приглашения (prompt)
string get_prompt() {
    return "🐱 " + filesystem::current_path().string() + "> ";
}


// ! Основная функция
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");
#endif

    print_welcome();

    string input;
    bool running = true;

    while (running) {
        cout << get_prompt();
        getline(cin, input);

        // ! Передаем команду на выполнение. Если вернулся false - выходим
        running = execute_command(input);
    }

    return 0;
}
