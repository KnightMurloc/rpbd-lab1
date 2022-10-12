//
// Created by victor on 15.09.2022.
//

#include <algorithm>
#include "Recipe.h"
#include <tuple>

void Recipe::add_ingridient(std::shared_ptr<Ingredient> ing, int count) {
    // ingridients.emplace_back(recipe,count);
    new_ingridients.emplace_back(ing,count);
}

void Recipe::remove_ingridient(std::shared_ptr<Ingredient> recipe) {
    new_ingridients.erase(std::remove_if(new_ingridients.begin(), new_ingridients.end(),
                                         [recipe](const std::tuple<std::shared_ptr<Ingredient>,int> &value){
                                             return std::get<0>(value)->get_id() == recipe->get_id();
                                         }), new_ingridients.end());
    removed_ingridients.emplace_back(recipe);
}

void Recipe::clear_removes_ingridients() {
    removed_ingridients = std::list<std::shared_ptr<Ingredient>>();
}

void Recipe::clear_new_ingridients() {
    new_ingridients = std::list<std::tuple<std::shared_ptr<Ingredient>,int>>();
}

const std::list<std::tuple<std::shared_ptr<Ingredient>,int>> Recipe::get_new_ingridients(){
    return new_ingridients;
}

const std::list<std::shared_ptr<Ingredient>> Recipe::get_removed_ingridients(){
    return removed_ingridients;
}
