# 🐱 Cat-Shell

**Cat-Shell** is a minimalist command shell with a feline aesthetic.  
It's simple, playful, and perfect for learning or just for fun.

```text
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.4
```

---

## 📦 Installation

```bash
git clone https://github.com/Cat-Shell/Cat-Shell
cd Cat-Shell
g++ -std=c++17 src/main.cpp src/commands.cpp -o cat_shell
sudo cp cat_shell /usr/local/bin/
```

---

## 🎮 Commands

| Command | Description |
|---------|-------------|
| `help`  | Show list of commands and help notes |
| `history` | Show the history of entered commands |
| `clear` | Clear the screen with a smooth wipe animation |
| `meow`  | Make the kitty meow or purr randomly |
| `echo [text]` | Print text to the screen |
| `cat [file]` | Display the contents of a text file |
| `kitty [emotion]` | Show ASCII kitty with an emotion (`sleep`, `happy`, `fright`) |
| `pwd`   | Show current working directory |
| `ls`    | List files and folders in the current directory |
| `cd [path]` | Change the current working directory (write without quotes) |
| `mkdir [name]` | Create a new folder |
| `rmdir [name]` | Delete an empty folder (asks for confirmation) |
| `rm [file]` | Delete a file (asks for confirmation) |
| `exit`  | Exit the shell and say goodbye to the kitty |

---

## 🧠 Example

```bash

  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.4

🐱 > help

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
  exit            - выйти из оболочки
  
Примеры:
  ls
  kitty sleep
  rm notes.txt
  echo привет, котик
  cat readme.txt

🐱 > history
1. ls
2. kitty sleep
3. echo привет, котик

🐱 > exit
Котик будет по тебе скучать(
```

---

## 💖 Support the Project

If you enjoy **Cat-Shell** and want to support its development, you can do so here:

[![DonationAlerts](https://img.shields.io/badge/DonationAlerts-Support-blue.svg)](https://www.donationalerts.com/r/felinefantasy)

Your support helps me:
- 🐱 Keep developing Cat-Shell
- 🌟 Add new features and commands
- ☕ Stay awake while coding at 4 AM

Every little bit is appreciated! ❤️

## 👥 Authors

- **FelineFantasy** — [GitHub](https://github.com/FelineFantasy)
- **SecDetxcoding** — [GitHub](https://github.com/SecDetxcoding)
- **License**: MIT
