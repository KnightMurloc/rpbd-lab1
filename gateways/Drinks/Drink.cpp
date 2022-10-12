//
// Created by victor on 15.09.2022.
//

#include "Drink.h"
#include "Drinkgateway.h"

Drink::Drink(int id) : id(id) {}

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

Recipe &Drink::getRecipe() {
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

int Drink::get_id() const {
    return id;
}

std::shared_ptr<Drink> Drink::get(int id){
    Drinkgateway gateway;
    return gateway.get(id);
}

std::list<std::shared_ptr<Drink>> Drink::get_great_than_by_id(int id, int count) {
    Drinkgateway gateway;
    return gateway.get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<Drink>> Drink::get_less_than_by_id(int id, int count) {
    Drinkgateway gateway;
    return gateway.get_less_then_by_id(id,count);
}

std::list<std::pair<int,int>> Drink::get_ingredients(std::shared_ptr<Drink> drink){
    Drinkgateway gateway;
    return gateway.get_ingredients(drink);
}

void Drink::save(std::shared_ptr<Drink> drink){
    Drinkgateway gateway;
    gateway.save(drink);
}

std::shared_ptr<Drink> Drink::create(std::string name, int strength, int size, std::string container,std::vector<std::pair<int,int>> ings){
    Drinkgateway gateway;
    return gateway.create(name,strength,size,container,ings);
}

void Drink::remove(std::shared_ptr<Drink> drink){
    Drinkgateway gateway;
    return gateway.remove(drink);
}
