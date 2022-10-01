//
// Created by victor on 15.09.2022.
//

#include <algorithm>
#include "Recipe.h"
#include <tuple>

void Recipe::add_ingridient(int recipe, int count) {
    // ingridients.emplace_back(recipe,count);
    new_ingridients.emplace_back(recipe,count);
}

void Recipe::remove_ingridient(int recipe) {
    new_ingridients.erase(std::remove_if(new_ingridients.begin(), new_ingridients.end(),
                                         [recipe](const std::tuple<int,int> &value){
                                             return std::get<0>(value) == recipe;
                                         }), new_ingridients.end());
    removed_ingridients.emplace_back(recipe);
}

void Recipe::clear_removes_ingridients() {
    removed_ingridients = std::list<int>();
}

void Recipe::clear_new_ingridients() {
    new_ingridients = std::list<std::tuple<int,int>>();
}

const std::list<std::tuple<int,int>> Recipe::get_new_ingridients(){
    return new_ingridients;
}

const std::list<int> Recipe::get_removed_ingridients(){
    return removed_ingridients;
}
