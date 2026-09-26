#include "commands.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <clocale>
#include <limits>
#include <random>
#include <sstream>

using namespace std;

#ifdef _WIN32
void init_win_console() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

void wait_dots(const string &msg) {
    cout << msg;
    for (int i = 0; i < 3; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    cout << "\n";
}

void print_welcome() {
    cout << R"(
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.4

)";
    cout << "Котик ждет твоей команды.\n\n";
}

void cmd_mkdir(const string& argument) {
    if (argument.empty()) { 
        cout << "Cat-Shell: котик не нашел название папки. Возможно ты ее не указал\n";
        return;
    }
    try {
        if (filesystem::create_directory(argument)) {
            cout << "Котик создал папку " << argument << endl;
        } else {
            cout << "Cat-Shell: твоя папка и так уже существует\n";
        }
    } catch (const filesystem::filesystem_error& e) {
        cout << "Котик обнаружил ошибку " << e.what() << endl;
    }
}

void cmd_rmdir(const string& argument) {
    if (argument.empty()) {
        cout << "Cat-Shell: котик не нашел название папки. Возможно ты ее не указал\n";
        return;
    }

    cout << "Котик собирается удалить папку \"" << argument << "\". Ты уверен? [y/N]: ";
    cout.flush();

    string answer;
    getline(cin, answer);

    if (answer != "y" && answer != "Y") {
        cout << "Котик передумал удалять папку" << endl;
        return;
    }

    if (!filesystem::exists(argument)) {
        cout << "Cat-Shell: пока ты думал, котик заметил, что такой папки и так не существует\n";
        return;
    }

    try {
        if (filesystem::remove(argument)) {
            cout << "Котик удалил папку " << argument << endl;
        } 
    } catch (const filesystem::filesystem_error& e) {
        cout << "Котик обнаружил ошибку (возможно, папка не пуста): " << e.what() << endl;
    }
}

void cmd_rm(const string& argument) {
    if (argument.empty()) {
        cout << "Cat-Shell: котик не нашел название файла. Возможно ты его не указал\n";
        return;
    }

    cout << "Котик собирается удалить файл \"" << argument << "\". Ты уверен? [y/N]: ";
    cout.flush();

    string answer;
    getline(cin, answer);

    if (answer != "y" && answer != "Y") {
        cout << "Котик передумал удалять файл" << endl;
        return;
    }

    if (!filesystem::exists(argument)) {
        cout << "Cat-Shell: пока ты думал, котик заметил, что такого файла и так не существует\n";
        return;
    }

    try {
        if (filesystem::remove(argument)) {
            cout << "Котик удалил файл " << argument << endl;
        }
    } catch (const filesystem::filesystem_error& e) {
        cout << "Котик обнаружил ошибку: " << e.what() << endl;
    }
}

void cmd_cd(const string& argument) {
    if (argument.empty()) {
        cout << "Cat-Shell: котик не нашел путь, ибо ты его не указал.\n";
        return;
    }
    try {
        filesystem::current_path(argument);
    } catch (const filesystem::filesystem_error& e) {
        cout << "Котик обнаружил ошибку: " << e.what() << endl;
    }
}

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
        cout << R"(
 /\_/\
( o.o )
 > ^ <
)" << "\n";
    }
}

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

void cmd_pwd() {
    cout << filesystem::current_path().string() << "\n";
}

void cmd_clear() {
    wait_dots("Котик уже бежит все слизывать");
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    print_welcome();
}

void cmd_history(const vector<string>& history) {
    if (history.empty()) {
        cout << "Котик пока ничего не запомнил" << endl;
        return;
    }
    for (size_t i = 0; i < history.size(); i++) {
        cout << (i + 1) << ". " << history[i] << endl;
    }
}

void cmd_meow() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 5);

    int number = dist(gen);

    if (number == 1) cout << "Мррр...\n";
    else if (number == 2) cout << "Мур-мяу!\n";
    else if (number == 3) cout << "Мяааау...\n";
    else if (number == 4) cout << "Мяу!\n";
    else cout << "мр~\n";
}

void cmd_echo(const string& argument) {
    cout << argument << "\n";
}

void cmd_cat(const string& argument) {
    if (argument.empty()) {
        cout << "Cat-Shell: котик не нашел название файла. Возможно ты его не указал\n";
        return;
    }

    ifstream file(argument);
    if (!file.is_open()) {
        cout << "Cat-Shell: котик не смог открыть файл \"" << argument << "\"\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        cout << line << "\n";
    }
    file.close();
}

void cmd_help() {
    wait_dots("Котик торопится достать листок с подсказками");
    cout << R"(
Доступные команды:
  help            - показать эту справку
  history         - показать историю комманд
  clear           - очистить экран
  meow            - мяукнуть
  echo [text]     - вывести текст на экран
  cat [file]      - вывести содержимое файла
  kitty [emotion] - показать котика с эмоцией
                  emotions: sleep, happy, fright
  pwd             - показать текущий путь
  ls              - список файлов и папок
  cd [path]       - поменять путь. Важно, писать без кавычек
  mkdir [name]    - создать папку
  rmdir [name]    - удалить пустую папку
  rm [file]       - удалить файл
  exit            - выйти из оболочки
  
Примеры:
  ls
  kitty sleep
  rm notes.txt
  echo привет, котик
  cat readme.txt
)";
}

bool execute_command(const string &input, const vector<string>& history) {
    if (input.empty()) return true;
    
    string command;
    string argument;

    istringstream iss(input);
    iss >> command;

    getline(iss, argument);
    if (!argument.empty() && argument[0] == ' ') {
        argument.erase(0, 1);
    }
    
    if (command == "exit") {
        cout << "Котик будет по тебе скучать(" << endl;
        return false;
    }

    if (command == "help") cmd_help();
    else if (command == "history") cmd_history(history);
    else if (command == "clear") cmd_clear();
    else if (command == "meow") cmd_meow();
    else if (command == "echo") cmd_echo(argument);
    else if (command == "cat") cmd_cat(argument);
    else if (command == "pwd") cmd_pwd();
    else if (command == "ls") cmd_ls();
    else if (command == "kitty") cmd_kitty(argument);
    else if (command == "cd") cmd_cd(argument);
    else if (command == "mkdir") cmd_mkdir(argument);
    else if (command == "rmdir") cmd_rmdir(argument);
    else if (command == "rm") cmd_rm(argument);
    else {
        cout << "Cat-Shell: команда не найдена: " << command << "\n";
        cout << "Котик не нашел ее" << endl;
    }

    return true;
}

string get_prompt() {
    string path = filesystem::current_path().string();
    return "\033[38;5;213m🐱 Cat-Shell\033[0m \033[38;5;245m" + path + "\033[0m \033[38;5;213m❯\033[0m ";
}
