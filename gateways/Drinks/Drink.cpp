//
// Created by victor on 15.09.2022.
//

#include "Drink.h"

Drink::Drink(int id, int r_id) : id(id), recipe(r_id) {}

const std::string &Drink::getName() const {
    return name;
}

int Drink::getStrength() const {
    return strength;
}

int Drink::getSize() const {
    return size;
}

const std::string &Drink::getContainer() const {
    return container;
}

const Recipe &Drink::getRecipe() const {
    return recipe;
}

void Drink::setName(const std::string &name) {
    Drink::name = name;
}

void Drink::setStrength(int strength) {
    Drink::strength = strength;
}

void Drink::setSize(int size) {
    Drink::size = size;
}

void Drink::setContainer(const std::string &container) {
    Drink::container = container;
}

int Drink::getId() const {
    return id;
}
