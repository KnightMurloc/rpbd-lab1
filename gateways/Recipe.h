//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_RECIPE_H
#define LAB1_RECIPE_H

#include <tuple>
#include <list>
#include <memory>
#include "Ingredients/Ingredient.h"

class Recipe{

private:
    std::list<std::tuple<std::shared_ptr<Ingredient>,int>> new_ingridients;
    std::list<std::shared_ptr<Ingredient>> removed_ingridients;
public:

    void add_ingridient(std::shared_ptr<Ingredient> ing, int count);
    void remove_ingridient(std::shared_ptr<Ingredient> recipe);

    void clear_removes_ingridients();
    void clear_new_ingridients();

    [[nodiscard]] const std::list<std::tuple<std::shared_ptr<Ingredient>,int>> get_new_ingridients();
    [[nodiscard]] const std::list<std::shared_ptr<Ingredient>> get_removed_ingridients();
};


#endif //LAB1_RECIPE_H
