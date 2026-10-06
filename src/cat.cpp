#include "cat.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

Cat cat;

std::chrono::steady_clock::time_point last_cat_update =
    std::chrono::steady_clock::now();

// обновляет состояние котика в зависимости от того,
// сколько времени прошло с последнего обновления
void update_cat() {

    auto now = std::chrono::steady_clock::now();

    auto seconds = std::chrono::duration_cast<
        std::chrono::seconds
    >(now - last_cat_update).count();

    // каждые 10 секунд котик немного теряет состояние
    if (seconds < 10)
        return;

    int ticks = seconds / 10;

    // базовая потеря состояния
    cat.satiety -= ticks * 2;
    cat.energy -= ticks;

    // если котик голодный — он быстрее грустит
    if (cat.satiety < 20)
        cat.happiness -= ticks * 2;
    else
        cat.happiness -= ticks;

    // если котик совсем голодный — у него быстрее кончаются силы
    if (cat.satiety == 0)
        cat.energy -= ticks;

    // если энергии совсем нет — котику становится очень грустно
    if (cat.energy == 0)
        cat.happiness -= ticks * 2;

    // не даем значениям уйти ниже нуля
    if (cat.satiety < 0)
        cat.satiety = 0;

    if (cat.energy < 0)
        cat.energy = 0;

    if (cat.happiness < 0)
        cat.happiness = 0;

    // сохраняем только реально прошедшие 10-секундные интервалы,
    // чтобы не терять оставшиеся секунды
    last_cat_update += std::chrono::seconds(ticks * 10);
}

// возвращает путь к файлу состояния котика
// ! важно: храним файл не в текущей папке, а в домашней директории пользователя
// иначе если запускать Cat-Shell из разных папок, будут заводить разные файлы
// и получится несколько разных котиков(

// путь примерно такой:
// linux/macOS: ~/.cat_shell/state
// винда:     %USERPROFILE%\.cat_shell\state
// если домашнюю папку определить не удалось, используем запасной вариант
// .cat_shell_save в текущей директории
static std::filesystem::path get_cat_state_path() {
    static const std::filesystem::path cached_path = [] {
        std::filesystem::path home;

#ifdef _WIN32
        // на винде сначала пробуем USERPROFILE
        // наскок помню это че то вроде C:\Users\Username
        const char* userprofile = std::getenv("USERPROFILE");
        if (userprofile && *userprofile) {
            home = userprofile;
        } else {
            // запасной вариант для стариных и дерьмовых (вся винда говно) конфигураций windows:
            const char* drive = std::getenv("HOMEDRIVE");
            const char* path = std::getenv("HOMEPATH");

            if (drive && path && *drive && *path) {
                home = std::string(drive) + std::string(path);
            }
        }
#else
        // на линуксе и макос юзаем типичную переменную HOME
        const char* home_env = std::getenv("HOME");
        if (home_env && *home_env) {
            home = home_env;
        }
#endif

        // если домвшнюю директорию найти не удалось, то не падаем
        // юзаем локальный файл в текущей папке
        if (home.empty()) {
            return std::filesystem::path(".cat_shell_save");
        }

        // создаем папку ~/.cat_shell, если ее еще нет
        // ошибку создания игнррируем, если не получится создать папку
        // save_cat_state() просто не сможет сохранить файл, но shell не упадет
        std::filesystem::path dir = home / ".cat_shell";

        std::error_code ec;
        std::filesystem::create_directories(dir, ec);

        return dir / "state";
    }();

    return cached_path;
}

// загружает состояние котика из файла
// формат файла 1
// satiety happiness energy

// первая строка - версич формата
// это нужно на будущее, если позже я\фелин добавлю новые поля
// сможем отличить старый файл от нового и сделать миграцмю
void load_cat_state() {
    std::ifstream file(get_cat_state_path());

    // если файла нет - , то похуй, это нормально
    // значит, запуск первый, и котик остается в стартовом состоянии 100/100/100.
    if (!file.is_open()) {
        // обновляем точку лтсчета времени, чтобы котик не начал голодвть
        // за то время, пока программа была закрыта
        last_cat_update = std::chrono::steady_clock::now();
        return;
    }

    int version = 0;

    // читаем версию формата
    // если не читается или версия неизвестна - игнорируем файл
    if (!(file >> version) || version != 1) {
        last_cat_update = std::chrono::steady_clock::now();
        return;
    }

    // значения по умолчанию, если вдруг с файлом чет случилось (например - повредилось)
    int satiety = 100;
    int happiness = 100;
    int energy = 100;

    // Пытаемся прочитать три числа
    if (file >> satiety >> happiness >> energy) {
        // std::clamp гарантирует, что значения будут в диапазоне 0 и 100
        // Даже если в файле будет -999 или 100000, котик не сломается к херам
        cat.satiety   = std::clamp(satiety, 0, 100);
        cat.happiness = std::clamp(happiness, 0, 100);
        cat.energy    = std::clamp(energy, 0, 100);
    }

    // после загрузки считаем, шо состояние актуально прямо сейчас
    last_cat_update = std::chrono::steady_clock::now();
}

// сохраняет состояние котика в файл
// Делаем запись безопасно -
// 1. Пишем во временный файл state.tm
// 2. Потом переименовываем его в state

// нахуя? - если программа упадет или выключится свет во время записи
// основной файл state не должен оказаться обрезанным/битым
void save_cat_state() {
    // перед сохранением применяем прошедшее время
    // нпмр, если котик был голодным, а игрок долго не открывал shell
    // то его состояние должно успеть ухудшиться перед записью
    update_cat();

    const std::filesystem::path final_path = get_cat_state_path();

    // временный файл рядом с основныб
    std::filesystem::path tmp_path = final_path;
    tmp_path += ".tmp";

    std::ofstream file(tmp_path);

    // если не удалось открыть временный файл для записи, значит выходим
    // шел не должен падать из-за того, что не получилось сохранить котика
    if (!file.is_open()) {
        return;
    }

    // ишем версию формата и текущее состояниекен я уже заебался
    file << 1 << '\n'
         << cat.satiety << ' '
         << cat.happiness << ' '
         << cat.energy << '\n';

    // рринудительно сбрасываем буферы в файл и закрываем его
    file.flush();
    file.close();

    std::error_code ec;

    // атомарно заменяем старый фвйл новым
    std::filesystem::rename(tmp_path, final_path, ec);

    // если rename не удался, пробуем запасной вариант
    if (ec) {
        std::error_code remove_ec;

        // удаляем старый файл, если он мешает
        std::filesystem::remove(final_path, remove_ec);

        // пробуем переименовать еще раз
        std::error_code rename_ec;
        std::filesystem::rename(tmp_path, final_path, rename_ec);

        // если снова не вышло, тр копируем файл как последний запасной вариант
        if (rename_ec) {
            std::error_code copy_ec;
            std::filesystem::copy_file(
                tmp_path,
                final_path,
                std::filesystem::copy_options::overwrite_existing,
                copy_ec
            );

            // удаляем временный файл в любом случае
            std::error_code cleanup_ec;
            std::filesystem::remove(tmp_path, cleanup_ec);
        }
    }
}

// показывает текущее состояние котика
void print_cat_status() {

    cout << "Сытость:  " << cat.satiety << "/100\n";
    cout << "Счастье:  " << cat.happiness << "/100\n";
    cout << "Энергия:  " << cat.energy << "/100\n";
}
