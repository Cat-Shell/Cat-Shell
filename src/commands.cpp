#include "commands.h" // подключает функции
#include <filesystem> // работа с файлами
#include <fstream> // чтение и запись файлов
#include <iostream> // ввод и вывод
#include <thread> // для sleep
#include <chrono> // для времени и задержки
#include <cstdlib> // систменые функции
#include <clocale> // локаль и кодировка
#include <limits> // ограничение типов
#include <random> // для случайных цифр
#include <sstream> // работа со строками как с потоками 

#ifdef _WIN32
#include <conio.h> // функции консоли windows
#else
#include <termios.h> // Linux/Unix
#include <unistd.h> // системные функции Linux/Unix
#endif

using namespace std;

// для подсветки
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

// анимация трех точек
void wait_dots(const string &msg) {
    cout << msg;
    for (int i = 0; i < 3; i++) {
        cout << ".";
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    cout << "\n";
}


// приветствие 
void print_welcome() {
    cout << R"(
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.5

)";
    cout << "Котик ждет твоей команды.\n\n";
}

// создать папку
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

// удалить папку
void cmd_rmdir(const string& argument) {
    if (argument.empty()) {
        cout << "Cat-Shell: котик не нашел название папки. Возможно ты ее не указал\n";
        return;
    }

    // точно ли удалить папку
    cout << "Котик собирается удалить папку \"" << argument << "\". Ты уверен? [y/N]: ";
    cout.flush();

    // получение ответа
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

// удаление файла
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

// указать путь и перейти
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

// наша функция, не связанная с ориг терминалом
// выводит эмоцию котика в зависимости от аргумента
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
    } else {  // ! если ничего не ввел или (пофиксить) -> после аргумента пробел
        cout << R"(
 /\_/\
( o.o )
 > ^ <
)" << "\n";
    }
}

// показывает какие есть файлы
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

// показать путь
void cmd_pwd() {
    cout << filesystem::current_path().string() << "\n";
}

// очистить все
void cmd_clear() {
    wait_dots("Котик уже бежит все слизывать");
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    print_welcome();
}

// показывает историю команд
void cmd_history(const vector<string>& history) {
    if (history.empty()) {
        cout << "Котик пока ничего не запомнил" << endl;
        return;
    }
    for (size_t i = 0; i < history.size(); i++) {
        cout << (i + 1) << ". " << history[i] << endl;
    }
}

// наша функция
// просто мяуканье
void cmd_meow() {
    // создание рандом числа
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 5);

    // засовываем число в переменную для различных манипуляций (ну в нашем случае просто if/else)
    int number = dist(gen);

    if (number == 1) cout << "Мррр...\n";
    else if (number == 2) cout << "Мур-мяу!\n";
    else if (number == 3) cout << "Мяааау...\n";
    else if (number == 4) cout << "Мяу!\n";
    else cout << "мр~\n";
}

// и так понятно, тута просто выводит то, что написал в аргумент пользователь
void cmd_echo(const string& argument) {
    cout << argument << "\n";
}

// выводит содержимое файла
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

    // вот тут и выводит
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

// и так ясно
void cmd_help() {
    wait_dots("Котик торопится достать листок с подсказками");
    cout << R"(
Доступные команды:
[Прочее]: 
  help            - показать эту справку
  version         - показать версию Cat-Shell
  history         - показать историю команд
  time            - показать текущую дату и время
  clear           - очистить экран
  exit            - выйти из оболочки

[Для работы]:  
  
  echo [text]     - вывести текст на экран
  cat [file]      - вывести содержимое файла
  pwd             - показать текущий путь
  ls              - список файлов и папок
  cd [path]       - поменять путь. Важно, писать без кавычек
  mkdir [name]    - создать папку
  rmdir [name]    - удалить пустую папку
  rm [file]       - удалить файл
  touch [file]    - создать пустой файл

[Команды Cat-Shell]: 
  meow            - мяукнуть
  kitty [emotion] - показать котика с эмоцией
                  emotions: sleep, happy, fright

Примеры:
  ls
  kitty sleep
  rm notes.txt
  echo привет, котик
  cat readme.txt
)";
}

// версия проекта
void cmd_version() {
    cout << "Cat-Shell v0.2.5\n";
    cout << "Котик доволен своей версией\n";
}


// создать файл
void cmd_touch(const string& argument) {
    if (argument.empty()) {
        cout << "Cat-Shell: котик не нашел название файла. Возможно ты его не указал\n";
        return;
    }

    if (filesystem::exists(argument)) {
        cout << "Cat-Shell: файл \"" << argument << "\" уже существует, котик его не трогал\n";
        return;
    }

    ofstream file(argument);
    if (!file.is_open()) {
        cout << "Cat-Shell: котик не смог создать файл \"" << argument << "\"\n";
        return;
    }
    file.close(); 
    cout << "Котик создал файл " << argument << endl;
}

// наверное, не нужная функция, ибо нсколько помню, оно нигде не используется
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

// свой ввод команты и аргумента с подсветкой синтаксиса
// я попросил ии сделать комментарии в этой функции
void redraw_input(
    const std::string& prompt,
    const std::string& input,
    std::size_t cursor
) {
    // Перерисовываем строку целиком
    cout << "\r\033[2K" << prompt;

    std::size_t space_pos = input.find(' ');

    // Если пробела нет — вся строка считается командой
    if (space_pos == std::string::npos) {
        cout << Color::command
             << input
             << Color::reset;
    } else {
        // До пробела красим как команду
        cout << Color::command
             << input.substr(0, space_pos)
             << Color::reset;

        cout << input.substr(space_pos, 1);

        // Всё после пробела — аргументы
        if (space_pos + 1 < input.size()) {
            cout << Color::argument
                 << input.substr(space_pos + 1)
                 << Color::reset;
        }
    }

    // Возвращаем курсор на нужную позицию
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

        // Если ввод закончился
        if (ch == EOF) {
            cout << '\n';
            break;
        }

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

    // Отключаем обычный режим ввода и вывод символов
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (true) {
        ch = getchar();

        // Если ввод закончился
        if (ch == EOF) {
            cout << '\n';
            break;
        }

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
                redraw_input(prompt, input, cursor);
            }
            continue;
        }

        // Escape sequence
        if (ch == '\033') {
            char second = getchar();

            if (second == EOF) break;

            if (second == '[') {
                char third = getchar();

                if (third == EOF) break;

                // ←
                if (third == 'D') {
                    if (cursor > 0) {
                        cursor--;
                        cout << "\033[D";
                        cout.flush();
                    }
                }

                // →
                else if (third == 'C') {
                    if (cursor < input.size()) {
                        cursor++;
                        cout << "\033[C";
                        cout.flush();
                    }
                }

                // Home
                else if (third == 'H') {
                    if (cursor > 0) {
                        cout << "\033[" << cursor << "D";
                        cursor = 0;
                        cout.flush();
                    }
                }

                // End
                else if (third == 'F') {
                    if (cursor < input.size()) {
                        cout << "\033[" << (input.size() - cursor) << "C";
                        cursor = input.size();
                        cout.flush();
                    }
                }

                // Delete: ESC [ 3 ~
                else if (third == '3') {
                    getchar(); // Поглощаем '~'

                    if (cursor < input.size()) {
                        input.erase(cursor, 1);
                        redraw_input(prompt, input, cursor);
                    }
                }
            }

            continue;
        }

        // Обычный символ
        input.insert(cursor, 1, ch);
        cursor++;
        redraw_input(prompt, input, cursor);
    }

    // Возвращаем настройки терминала обратно
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

#endif

    return input;
}

// проверяет, есть ли команда в PATH
bool command_exists(const string& cmd) {
    namespace fs = std::filesystem;

    // если путь абсолютный/относительный — просто проверим файл
    if (cmd.find('/') != string::npos
#ifdef _WIN32
        || cmd.find('\\') != string::npos
#endif
    ) {
        return fs::exists(cmd);
    }

    const char* path_env = getenv("PATH");
    if (!path_env) return false;

#ifdef _WIN32
    const char sep = ';';
    const vector<string> exts = {".exe", ".bat", ".cmd", ".com", ""};
#else
    const char sep = ':';
    const vector<string> exts = {""};
#endif

    string path_str = path_env;
    size_t start = 0, end;

    auto check_dir = [&](const string& dir) -> bool {
        for (const auto& ext : exts) {
            fs::path candidate = fs::path(dir) / (cmd + ext);
            if (fs::exists(candidate) && fs::is_regular_file(candidate)) {
#ifndef _WIN32
                auto perms = fs::status(candidate).permissions();
                if ((perms & fs::perms::owner_exec) == fs::perms::none &&
                    (perms & fs::perms::group_exec) == fs::perms::none &&
                    (perms & fs::perms::others_exec) == fs::perms::none)
                    continue;
#endif
                return true;
            }
        }
        return false;
    };

    while ((end = path_str.find(sep, start)) != string::npos) {
        if (check_dir(path_str.substr(start, end - start))) return true;
        start = end + 1;
    }
    return check_dir(path_str.substr(start));
}

// запускает внешнюю команду целиком (имя + аргументы)
void cmd_exec(const string& input) {
    if (input.empty()) return;

    int status = std::system(input.c_str());

    if (status == -1) {
        cout << "Cat-Shell: котик не смог запустить: " << input << "\n";
    }
}

// проверка команды
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
    else if (command == "version") cmd_version();
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
    else if (command == "touch") cmd_touch(argument);
    else {
        // не встроенная — пробуем запустить как внешнюю
        if (command_exists(command)) {
            cmd_exec(input);
        } else {
            cout << "Cat-Shell: команда не найдена: " << command << "\n";
            cout << "Котик не нашел ее" << endl;
        }
    }

    return true;
}

// и так ясно)
string get_prompt() {
    string path = filesystem::current_path().string();
    
    return Color::pink + "🐱 Cat-Shell" + Color::reset + " "
        + Color::gray + path + Color::reset
        + " " + Color::pink + "❯" + Color::reset + " ";
}
