#ifndef COMMANDS_H
#define COMMANDS_H

#define NOMINMAX // ! Обязательно ДО #include <Windows.h>

#include <string>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
void init_win_console();
#endif

// Утилиты интерфейса
void wait_dots(const std::string &msg);
void print_welcome();
std::string get_prompt();

// Команды оболочки
void cmd_mkdir(const std::string& argument);
void cmd_rmdir(const std::string& argument);
void cmd_rm(const std::string& argument);
void cmd_cd(const std::string& argument);
void cmd_kitty(const std::string& argument);
void cmd_ls();
void cmd_pwd();
void cmd_clear();
void cmd_history(const std::vector<std::string>& history);
void cmd_meow();
void cmd_echo(const std::string& argument);
void cmd_cat(const std::string& argument);
void cmd_help();

// Обработчик команд
bool execute_command(const std::string &input, const std::vector<std::string>& history);

#endif // COMMANDS_H
