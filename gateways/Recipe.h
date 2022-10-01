//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_RECIPE_H
#define LAB1_RECIPE_H

#include <tuple>
#include <list>

class Recipe{

private:
    std::list<std::tuple<int,int>> new_ingridients;
    std::list<int> removed_ingridients;
public:

    void add_ingridient(int recipe, int count);
    void remove_ingridient(int recipe);

    void clear_removes_ingridients();
    void clear_new_ingridients();

    [[nodiscard]] const std::list<std::tuple<int,int>> get_new_ingridients();
    [[nodiscard]] const std::list<int> get_removed_ingridients();
};


#endif //LAB1_RECIPE_H
