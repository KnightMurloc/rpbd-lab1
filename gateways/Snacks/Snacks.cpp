//
// Created by victor on 11.09.2022.
//

#include "Snacks.h"
#include <tuple>
#include <algorithm>
#include "../Ingredients/IngredientGateway.h"
#include "fmt/core.h"
#include <fmt/format.h>


int Snack::get_id() const {
    return id;
}

const std::string &Snack::get_name() const {
    return name;
}

int Snack::get_size() const {
    return size;
}

Recipe &Snack::get_recipe() {
    return recipe;
}

void Snack::set_name(const std::string &name) {
    this->name = name;
}

void Snack::set_size(int size) {
    this->size = size;
}

Snack::Snack(int id, int recipe_id) : id(id), recipe(recipe_id) {}


