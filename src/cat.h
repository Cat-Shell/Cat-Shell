#ifndef CAT_H
#define CAT_H

#include <chrono>

struct Cat {

    int satiety = 100;
    int happiness = 100;
    int energy = 100;

};

extern Cat cat;
extern std::chrono::steady_clock::time_point last_cat_update;

void update_cat();
void load_cat_state();
void save_cat_state();
void print_cat_status();

#endif // CAT_H
