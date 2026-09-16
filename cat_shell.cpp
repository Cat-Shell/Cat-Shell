#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <random> // ! библиотека для рандом числа
#include <sstream>

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


// ! выводит ASCII котиков
void cmd_kitty(const string& argument) {

    if (argument == "sleep") {

        cout << R"(
 /\_/\
( -.- )
 > ^ <
)" << "\n";

    } else if (argument == "happy") {

        cout << R"(
 /\_/\
( ^.^ )
 > ^ <
)" << "\n";

    } else if (argument == "fright") {

        cout << R"(
 /\_/\
( O.O )
 > ^ <
)" << "\n";

    } else {
        //! kitty без аргумента
        cout << R"(
 /\_/\
( o.o )
 > ^ <
)" << "\n";

    }

}

// ! Вывод списка файлов и папок
void cmd_ls() {

    wait_dots("Котенок перебирает файлы");

    try {
        for (const auto &entry : filesystem::directory_iterator(filesystem::current_path())) {
            cout << entry.path().filename().string() << "\n";
        }
    } catch (const filesystem::filesystem_error &e) {
        cout << "Ошибка: " << e.what() << "\n";
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

    print_welcome();
}

// ! Мяуканье
void cmd_meow() {

    // ! функции для создания рандом числа
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 5);

    // ! рандомное число перемещаем в переменную number
    int number = dist(gen);

    if (number == 1)
        cout << "Мррр...\n";
    else if (number == 2)
        cout << "Мур-мяу!\n";
    else if (number == 3)
        cout << "Мяааау...\n";
    else if (number == 4)
        cout << "Мяу!\n";
    else
        cout << "мр~\n";

}

// ! Печать справки по командам
void cmd_help() {

    wait_dots("Котик торопиться достать листок с подсказками");

    cout << R"(
Доступные команды:
  help           - показать эту справку
  clear          - очистить экран
  meow           - мяукнуть
  kitty          - эмоция котика
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
    } 
    
    string command;
    string argument;

    istringstream iss(input);
    iss >> command;
    iss >> argument;
    
    if (command == "exit") {
        cout << "Котик будет по тебе скучать(" << endl;
        return false;
    }

    if (command == "help") {
        cmd_help();

    } else if (command == "clear") {
        cmd_clear();

    } else if (command == "meow") {
        cmd_meow();

    } else if (command == "pwd") {
        cmd_pwd();

    } else if (command == "ls") {
        cmd_ls();

    } else if (command == "kitty") {
        cmd_kitty(argument);

    } else {
        cout << "Cat-Shell: команда не найдена: " << command << "\n";
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

        if (!getline(cin, input)) {
            break;
        }

        // ! Передаем команду на выполнение. Если вернулся false - выходим
        running = execute_command(input);
    }

    return 0;
}
