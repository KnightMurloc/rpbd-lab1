//
// Created by victor on 12.09.2022.
//

#include "Ingredient.h"

int Ingredient::get_id() const {
    return id;
}

const std::string &Ingredient::get_name() const {
    return name;
}

Unit Ingredient::get_unit() const {
    return unit;
}

void Ingredient::set_name(const std::string &name) {
    Ingredient::name = name;
}

void Ingredient::set_unit(Unit unit) {
    Ingredient::unit = unit;
}

Ingredient::Ingredient(int id) : id(id) {}
