#ifndef COMMANDS_H
#define COMMANDS_H

#define NOMINMAX // ! Обязательно ДО #include <Windows.h>

#include <string>
#include <vector>
#include <iomanip>
#include <cstddef>
#include <atomic>
#include <csignal>

extern std::atomic<bool> g_interrupted;
extern "C" void sigint_handler(int);

// коротко записанные цвета
namespace Color {
    const std::string pink = "\033[38;5;213m";
    const std::string gray = "\033[38;5;245m";

    const std::string command = "\033[38;5;99m"; // яркий индиго 
    const std::string argument = "\033[38;5;197m"; // малиновый

    const std::string reset = "\033[0m";
}

#ifdef _WIN32
#include <Windows.h>
void init_win_console();
#endif

// Утилиты интерфейса
void wait_dots(const std::string &msg);
void print_welcome();
std::string get_prompt();
std::string read_input(const std::string& prompt);

// свой ввод
void print_highlighted_command(
    const std::string& command,
    const std::string& argument
);
 
void redraw_input(
    const std::string& prompt,
    const std::string& input,
    std::size_t cursor
);

// Команды оболочки
void cmd_mkdir(const std::string& argument);
void cmd_rmdir(const std::string& argument);
void cmd_rm(const std::string& argument);
void cmd_cd(const std::string& argument);
void cmd_pet(const std::string& argument);
void cmd_kitty(const std::string& argument);
void print_cat_status();
void load_cat_state(); // загрузка и сохранение состояние котика
void save_cat_state(); //   чтобы котик помнил себя между запусками
void cmd_ls();
void cmd_pwd();
void cmd_clear();
void cmd_history(const std::vector<std::string>& history);
void cmd_meow();
void cmd_echo(const std::string& argument);
void cmd_cat(const std::string& argument);
void cmd_time();
void cmd_help();
void cmd_version();
void cmd_touch(const std::string& argument);

// Внешние команды
bool command_exists(const std::string& cmd);
void cmd_exec(const std::string& input);

// Обработчик команд
bool execute_command(const std::string &input, const std::vector<std::string>& history);

#endif // COMMANDS_H