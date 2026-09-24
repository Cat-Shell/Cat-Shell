#define NOMINMAX // ! Обязательно ДО #include <Windows.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <clocale>
#include <limits> // ! библиотека для очистки оставшегося ввода
#include <random> // ! библиотека для рандом числа
#include <sstream>  // ! библиотека для аргумента
#include <vector> // ! библиотека для команды history

#ifdef _WIN32

#include <Windows.h>

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
  > ^ <   Cat-Shell v0.2.4

)";
    cout << "Котик ждет твоей команды.\n\n";
}

// ! создает папку
void cmd_mkdir(const string& argument) {

    // ! проверка на то, пуст ли аргумент
    if (argument.empty()) { 
        cout << "Cat-Shell: котик не нашел название папки. Возможно ты ее не указал\n";
        return; // ! завершаем работу функции
    }

    try {
        if (filesystem::create_directory(argument)) {
            cout << "Котик создал папку " << argument << endl;
        } else {
            cout << "Cat-Shell: твоя папка и так уже существует\n";
        }
    } catch (const filesystem::filesystem_error& e) {
        cout << "Котик обнаружил ошибку " << e.what() << endl; // ! вывод ошибки
    }

}

// ! удаляет пустую папку
void cmd_rmdir(const string& argument) {

    if (argument.empty()) {
        cout << "Cat-Shell: котик не нашел название папки. Возможно ты ее не указал\n";
        return;
    }

    cout << "Котик собирается удалить папку \"" << argument
         << "\". Ты уверен? [y/N]: ";
    cout.flush();

    string answer;
    getline(cin, answer);   // ! читаем всю строку целиком

    if (answer != "y" && answer != "Y") {
        cout << "Котик передумал удалять папку" << endl;
        return;
    }

    try {
        if (filesystem::remove(argument)) {
            cout << "Котик удалил папку " << argument << endl;
        } else {
            cout << "Cat-Shell: такой папки и так не существовало\n";
        }
    } catch (const filesystem::filesystem_error& e) {
        cout << "Котик обнаружил ошибку (возможно, папка не пуста): "
             << e.what() << endl;
    }
}

// ! удаляет файл
void cmd_rm(const string& argument) {

    if (argument.empty()) {
        cout << "Cat-Shell: котик не нашел название файла. Возможно ты его не указал\n";
        return;
    }

    cout << "Котик собирается удалить файл \"" << argument
         << "\". Ты уверен? [y/N]: ";
    cout.flush();

    string answer;
    getline(cin, answer);

    if (answer != "y" && answer != "Y") {
        cout << "Котик передумал удалять файл" << endl;
        return;
    }

    try {
        if (filesystem::remove(argument)) {
            cout << "Котик удалил файл " << argument << endl;
        } else {
            cout << "Cat-Shell: такого файла и так не существовало\n";
        }
    } catch (const filesystem::filesystem_error& e) {
        cout << "Котик обнаружил ошибку: " << e.what() << endl;
    }
}

// ! указать путь
void cmd_cd(const string& argument) {

    // ! проверка на то, пустой ли аргумент
    if (argument.empty()) {
        cout << "Cat-Shell: котик не нашел путь, ибо ты его не указал.\n";
        return; // ! означает, что функция завершила работу
    }

    try {
        filesystem::current_path(argument);     // ! изменяет текущую директорию
    } catch (const filesystem::filesystem_error& e) {
        cout << "Котик обнаружил ошибку: " << e.what() << endl; // ! если чот произойдет, то выведет ошибку
    }

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
        // ! kitty без аргумента
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

// ! история введенных команд
void cmd_history(const vector<string>& history) {
    
    if (history.empty()) {
        cout << "Котик пока ничего не запомнил" << endl;
        return;
    }

    for (size_t i = 0; i < history.size(); i++) {
        cout << (i + 1) << " " << history[i] << endl;
    }

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

// ! Эхо: выводит переданный текст
void cmd_echo(const string& argument) {
    cout << argument << "\n";
}

// ! Печать справки по командам
void cmd_help() {

    wait_dots("Котик торопится достать листок с подсказками");

    cout << R"(
Доступные команды:
  help            - показать эту справку
  history         - показать историю комманд
  clear           - очистить экран
  meow            - мяукнуть
  echo [text]     - вывести текст на экран
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
)";
}

// ! Обработка и выполнение команд
bool execute_command(const string &input, const vector<string>& history) {
    if (input.empty()) {
        return true; // ! Пропуск пустой строки
    } 
    
    string command;
    string argument;

    istringstream iss(input);
    iss >> command;

    // ! Читаем остаток строки целиком, чтобы аргумент мог содержать пробелы
    getline(iss, argument);
    if (!argument.empty() && argument[0] == ' ') {
        argument.erase(0, 1); // ! убираем ведущий пробел
    }
    
    if (command == "exit") {
        cout << "Котик будет по тебе скучать(" << endl;
        return false;
    }

    if (command == "help") {
        cmd_help();

    } else if (command == "history") {
        cmd_history(history);

    } else if (command == "clear") {
        cmd_clear();

    } else if (command == "meow") {
        cmd_meow();

    } else if (command == "echo") {
        cmd_echo(argument);

    } else if (command == "pwd") {
        cmd_pwd();

    } else if (command == "ls") {
        cmd_ls();

    } else if (command == "kitty") {
        cmd_kitty(argument);

    } else if (command == "cd") {
        cmd_cd(argument);
    } else if (command == "mkdir") {
        cmd_mkdir(argument);
    } else if (command == "rmdir") {
        cmd_rmdir(argument);
    } else if (command == "rm") {
        cmd_rm(argument);
    } else {
        cout << "Cat-Shell: команда не найдена: " << command << "\n";
        cout << "Котик не нашел ее" << endl;
    }

    return true;
}

// ! Формирование строки приглашения (prompt)
string get_prompt() {
    string path = filesystem::current_path().string();

    return "\033[38;5;213m🐱 Cat-Shell\033[0m "
           "\033[38;5;245m" + path + "\033[0m "
           "\033[38;5;213m❯\033[0m ";
}


// ! Основная функция
int main() {
#ifdef _WIN32
    init_win_console();
    setlocale(LC_ALL, ".UTF8");
#endif

    print_welcome();

    string input;
    bool running = true;
    vector<string> history;

    while (running) {
        cout << get_prompt() << flush;

        if (!getline(cin, input)) {
            break;
        }

        history.push_back(input);

        // ! Передаем команду на выполнение. Если вернулся false - выходим
        running = execute_command(input, history);
    }

    return 0;
}
