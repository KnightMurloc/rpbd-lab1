//
// Created by victor on 12.09.2022.
//

#include "Ingredient.h"
#include "IngredientGateway.h"



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

std::shared_ptr<Ingredient> Ingredient::get(int id){
    IngredientGateway gateway;
    return gateway.get(id);
}

std::list<std::shared_ptr<Ingredient>> Ingredient::get_great_than_by_id(int id, int count) {
    IngredientGateway gateway;
    return gateway.get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<Ingredient>> Ingredient::get_less_than_by_id(int id, int count) {
    IngredientGateway gateway;
    return gateway.get_less_then_by_id(id,count);
}

void Ingredient::save(std::shared_ptr<Ingredient> ing){
    IngredientGateway gateway;
    gateway.save(ing);
}

std::shared_ptr<Ingredient> Ingredient::create(std::string name, Unit unit){
    IngredientGateway gateway;
    return gateway.create(name,unit);
}

void Ingredient::remove(std::shared_ptr<Ingredient> ing){
    IngredientGateway gateway;
    return gateway.remove(ing);
}


std::list<std::shared_ptr<Ingredient>> Ingredient::get_great_then_by_id_filtred_by_name(std::string name, int id, int count){
    IngredientGateway gateway;
    return gateway.get_great_then_by_name(name,id,count);
}

std::list<std::shared_ptr<Ingredient>> Ingredient::get_less_then_by_id_filtred_by_name(std::string name, int id, int count){
    IngredientGateway gateway;
    return gateway.get_less_then_by_name(name,id,count);
}
