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

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

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
  > ^ <   Cat-Shell v0.2.5

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

// ! Выводит текущую дату и время
void cmd_time() {
    wait_dots("Котик смотрит на настенные часы");

    // Получаем текущее системное время
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);

    // Безопасно переводим в локальное время (работает и на Windows, и на Linux)
    tm buf;
#ifdef _WIN32
    localtime_s(&buf, &in_time_t);
#else
    localtime_r(&in_time_t, &buf);
#endif

    // Красиво выводим время
    cout << "Сейчас: " << put_time(&buf, "%Y-%m-%d %H:%M:%S") << "\n";
}

void cmd_help() {
    wait_dots("Котик торопится достать листок с подсказками");
    cout << R"(
Доступные команды:
  help            - показать эту справку
  history         - показать историю команд
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
  time            - показать текущую дату и время
  exit            - выйти из оболочки
  
Примеры:
  ls
  kitty sleep
  rm notes.txt
  echo привет, котик
  cat readme.txt
)";
}

void print_highlighted_command(
    const std::string& command,
    const std::string& argument
) {

    cout << Color::command << command << Color::reset;
    
    if (!argument.empty()) {
        cout << " "
            << Color::argument << argument << Color::reset;
    }

    cout << "\n";

}

void redraw_input(
    const std::string& prompt,
    const std::string& input,
    std::size_t cursor
) {
    cout << "\r\033[2K" << prompt;

    std::size_t space_pos = input.find(' ');

    if (space_pos == std::string::npos) {
        // Введена только команда
        cout << Color::command << input << Color::reset;
    } else {
        // Команда
        cout << Color::command
             << input.substr(0, space_pos)
             << Color::reset;

        // Пробел и аргумент
        cout << input.substr(space_pos, input.size() - space_pos);

        if (space_pos + 1 < input.size()) {
            cout << Color::argument
                 << input.substr(space_pos + 1)
                 << Color::reset;
        }
    }

    // Возвращаем курсор туда, где пользователь сейчас находится
    if (cursor < input.size()) {
        cout << "\033[" << (input.size() - cursor) << "D";
    }

    cout.flush();
}

std::string read_input(const std::string& prompt) {
    std::string input;
    std::size_t cursor = 0;
    char ch;

#ifdef _WIN32

    while (true) {
        ch = _getch();

        // Enter
        if (ch == '\r') {
            cout << '\n';
            break;
        }

        // Backspace
        if (ch == '\b') {
            if (cursor > 0) {
                input.erase(cursor - 1, 1);
                cursor--;

                redraw_input(prompt, input, cursor);
            }
            continue;
        }

        // Специальные клавиши Windows
        if (ch == 0 || ch == 224) {
            ch = _getch();

            // ←
            if (ch == 75) {
                if (cursor > 0) {
                    cursor--;
                    cout << "\033[D";
                }
            }

            // →
            else if (ch == 77) {
                if (cursor < input.size()) {
                    cursor++;
                    cout << "\033[C";
                }
            }

            // Home
            else if (ch == 71) {
                if (cursor > 0) {
                    cout << "\033[" << cursor << "D";
                    cursor = 0;
                }
            }

            // End
            else if (ch == 79) {
                if (cursor < input.size()) {
                    cout << "\033[" << (input.size() - cursor) << "C";
                    cursor = input.size();
                }
            }

            // Delete
            else if (ch == 83) {
                if (cursor < input.size()) {
                    input.erase(cursor, 1);

                    redraw_input(prompt, input, cursor);
                }
            }

            continue;
        }

        // Обычный символ
        input.insert(cursor, 1, ch);
        cursor++;

        redraw_input(prompt, input, cursor);
    }

#else

    termios oldt{};
    termios newt{};

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (true) {
        ch = getchar();

        // Enter
        if (ch == '\n') {
            cout << '\n';
            break;
        }

        // Backspace
        if (ch == 127) {
            if (cursor > 0) {
                input.erase(cursor - 1, 1);
                cursor--;

                cout << "\r\033[2K"
                     << prompt
                     << input;

                if (cursor < input.size()) {
                    cout << "\033[" << (input.size() - cursor) << "D";
                }
                cout.flush();
            }
            continue;
        }

        // Escape sequence
        if (ch == '\033') {
            char second = getchar();

            if (second == '[') {
                char third = getchar();

                // ←
                if (third == 'D') {
                    if (cursor > 0) {
                        cursor--;
                        cout << "\033[D";
                    }
                }

                // →
                else if (third == 'C') {
                    if (cursor < input.size()) {
                        cursor++;
                        cout << "\033[C";
                    }
                }

                // Home
                else if (third == 'H') {
                    if (cursor > 0) {
                        cout << "\033[" << cursor << "D";
                        cursor = 0;
                    }
                }

                // End
                else if (third == 'F') {
                    if (cursor < input.size()) {
                        cout << "\033[" << (input.size() - cursor) << "C";
                        cursor = input.size();
                    }
                }

                // Delete: ESC [ 3 ~
                else if (third == '3') {
                    getchar(); // '~'

                    if (cursor < input.size()) {
                        input.erase(cursor, 1);

                        cout << "\r\033[2K"
                             << prompt
                             << input;

                        if (cursor < input.size()) {
                            cout << "\033[" << (input.size() - cursor) << "D";
                        }
                        cout.flush();
                    }
                }
            }

            continue;
        }

        // Обычный символ
        input.insert(cursor, 1, ch);
        cursor++;

        cout << "\r\033[2K"
             << prompt
             << input;

        if (cursor < input.size()) {
            cout << "\033[" << (input.size() - cursor) << "D";
        }

        cout.flush();
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

#endif

    return input;
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
    else if (command == "time") cmd_time();
    else {
        cout << "Cat-Shell: команда не найдена: " << command << "\n";
        cout << "Котик не нашел ее" << endl;
    }

    return true;
}

string get_prompt() {
    string path = filesystem::current_path().string();
    
    return Color::pink + "🐱 Cat-Shell" + Color::reset + " "
        + Color::gray + path + Color::reset
        + " " + Color::pink + "❯" + Color::reset + " ";
}
