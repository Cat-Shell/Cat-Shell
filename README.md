# 🐱 Cat-Shell

**Cat-Shell** is a minimalist command shell with a feline aesthetic.  
It's simple, playful, and perfect for learning or just for fun.

```text
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.3
```

---

## 📦 Installation

```bash
git clone https://github.com/Cat-Shell/Cat-Shell
cd Cat-Shell
g++ -std=c++17 -o cat_shell cat_shell.cpp
sudo cp cat_shell /usr/local/bin/
```

---

## 🎮 Commands

| Command | Description |
|---------|-------------|
| `help`  | Show list of commands |
| `clear` | Clear the screen |
| `meow`  | Meow |
| `pwd`   | Show current working directory |
| `ls`    | List files and folders in the current directory |
| `cd`    | Change the current working directory |
| `mkdir` | Create a new folder |
| `rmdir` | Delete an empty folder |
| `exit`  | Exit the shell |

---

## 🧠 Example

```bash

  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.3

🐱 > help

Доступные команды:
  help            - показать эту справку
  clear           - очистить экран
  meow            - мяукнуть
  kitty [emotion] - показать котика с эмоцией
                  emotions: sleep, happy, fright
  pwd             - показать текущий путь
  ls              - список файлов и папок
  cd [path]       - поменять путь. Важно, писать без кавычек
  mkdir [name]    - создать папку
  rmdir [name]    - удалить пустую папку
  exit            - выйти из оболочки
  
Примеры:
  ls
  kitty sleep

🐱 > mkdir test_folder
Котик создал папку test_folder

🐱 > rmdir test_folder
Котик успешно удалил папку test_folder

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
