# 🐱 Cat-Shell

**Cat-Shell** — это минималистичная командная оболочка с кошачьей эстетикой.  
Она простая, игривая и идеально подходит для изучения или просто для удовольствия.

```text
  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.1
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
| `help`  | Показать список команд |
| `clear` | Очистить экран |
| `meow`  | Мяукнуть |
| `pwd`   | Показать текущий путь |
| `ls`    | Список файлов и папок в текущей директории |
| `cd <path>` | Сменить текущую директорию |
| `exit`  | Выйти из оболочки |

---

## 🧠 Example

```bash
$ cat-shell

  /\_/\
 ( o.o )
  > ^ <   Cat-Shell v0.2.1

🐱 /home/user > help
help  - эта справка
clear - очистить экран
meow  - мяукнуть
pwd   - текущий путь
ls    - список файлов и папок
cd    - сменить директорию
exit  - выйти

🐱 /home/user > meow
Мяу!

🐱 /home/user > cd Documents
🐱 /home/user/Documents > pwd
/home/user/Documents

🐱 /home/user/Documents > exit
```

---

## 👥 Authors

- **FelineFantasy** — [GitHub](https://github.com/FelineFantasy)
- **SecDetxcoding** — [GitHub](https://github.com/SecDetxcoding)
- **License**: MIT
