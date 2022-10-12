//
// Created by victor on 11.09.2022.
//

#include "Snacks.h"
#include <tuple>
#include <algorithm>
#include "../Ingredients/IngredientGateway.h"
#include "SnackGateway.h"
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

void Snack::set_name(const std::string &name) {
    this->name = name;
}

void Snack::set_size(int size) {
    this->size = size;
}

Recipe& Snack::get_recipe(){
    return recipe;
}

Snack::Snack(int id) : id(id) {}

std::shared_ptr<Snack> Snack::get(int id){
    SnackGateway gateway;
    return gateway.get(id);
}

std::list<std::shared_ptr<Snack>> Snack::get_great_than_by_id(int id, int count) {
    SnackGateway gateway;
    return gateway.get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<Snack>> Snack::get_less_than_by_id(int id, int count) {
    SnackGateway gateway;
    return gateway.get_less_then_by_id(id,count);
}

std::list<std::pair<int,int>> Snack::get_ingredients(std::shared_ptr<Snack> data){
    SnackGateway gateway;
    return gateway.get_ingredients(data);
}

void Snack::remove(std::shared_ptr<Snack> snack){
    SnackGateway gateway;
    gateway.remove(snack);
}

void Snack::save(std::shared_ptr<Snack> snack){
    SnackGateway gateway;
    gateway.save(snack);
}

std::shared_ptr<Snack> Snack::create(std::string name, int size, std::vector<std::pair<int,int>> ings){
    SnackGateway gateway;
    return gateway.create(name,size,ings);
}
