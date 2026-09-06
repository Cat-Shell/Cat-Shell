#include <filesystem>
#include <iostream>
#include <string>
#include <cstdlib>

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace std;

// Печать приветственного сообщения
void print_welcome() {
    cout << R"(
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.1

)";
}

// Вывод списка файлов и папок
void cmd_ls() {
    for (const auto &entry : filesystem::directory_iterator(filesystem::current_path())) {
        cout << entry.path().filename().string() << "\n";
    }
}

// Печать текущего пути
void cmd_pwd() {
    cout << filesystem::current_path().string() << "\n";
}

// Очистка экрана
void cmd_clear() {
    cout << "\033[H\033[J";
}

// Мяуканье
void cmd_meow() {
    cout << "Мяу!\n";
}

// Печать справки по командам
void cmd_help() {
    cout << "help - эта справка\n";
    cout << "clear - очистить экран\n";
    cout << "meow - мяукнуть\n";
    cout << "pwd - текущий путь\n";
    cout << "ls - список файлов и папок\n";
    cout << "exit - выйти\n";
}

// Обработка и выполнение команд
bool execute_command(const string &input) {
    if (input.empty()) {
        return true; // Пропуск пустой строки
    }
    if (input == "exit") {
        return false; // Сигнал для выхода из цикла
    }
    
    if (input == "help") {
        cmd_help();
    } else if (input == "meow") {
        cmd_meow();
    } else if (input == "clear") {
        cmd_clear();
    } else if (input == "pwd") {
        cmd_pwd();
    } else if (input == "ls") {
        cmd_ls();
    } else {
        cout << "Cat-Shell: команда не найдена: " << input << "\n";
    }
    
    return true;
}

// Формирование строки приглашения (prompt)
string get_prompt() {
    return "🐱 " + filesystem::current_path().string() + " > ";
}

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
        
        // Передаем команду на выполнение. Если вернулся false - выходим
        running = execute_command(input);
    }

    return 0;
}
