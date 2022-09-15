//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_DRINK_H
#define LAB1_DRINK_H

#include <string>
#include "../Recipe.h"

class Drink {
private:
    int id;
    std::string name;
    int strength;
    int size;
    std::string container;
    Recipe recipe;
public:
    Drink(int id, int r_id);

    int getId() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] int getStrength() const;

    [[nodiscard]] int getSize() const;

    [[nodiscard]] const std::string &getContainer() const;

    [[nodiscard]] Recipe &getRecipe();

    void setName(const std::string &name);

    void setStrength(int strength);

    void setSize(int size);

    void setContainer(const std::string &container);
};


#endif //LAB1_DRINK_H
