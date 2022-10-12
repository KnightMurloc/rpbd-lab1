//
// Created by victor on 02.10.2022.
//

#include "DrinkOrder.h"
#include "../Drinks/Drinkgateway.h"
#include "../Employeer/EmployeerGateway.h"
#include "DrinkOrderGateway.h"

int DrinkOrder::get_id() const {
    return id;
}

DrinkOrder::DrinkOrder(int id) : id(id) {}

int DrinkOrder::get_table() const {
    return table;
}

void DrinkOrder::set_table(int table) {
    DrinkOrder::table = table;
}

std::shared_ptr<Drink> DrinkOrder::get_drink() {
    return drink;
}

std::shared_ptr<Employeer> DrinkOrder::get_waiter() {
    EmployeerGateway gateway;
    return waiter;
}

void DrinkOrder::set_drink(std::shared_ptr<Drink> drink){
    this->drink = drink;
}

void DrinkOrder::set_waiter(std::shared_ptr<Employeer> waiter){
    this->waiter = waiter;
}

std::list<std::shared_ptr<DrinkOrder>> DrinkOrder::get_great_than_by_id(int id, int count) {
    DrinkOrderGateway gateway;
    return gateway.get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<DrinkOrder>> DrinkOrder::get_less_than_by_id(int id, int count) {
    DrinkOrderGateway gateway;
    return gateway.get_less_then_by_id(id,count);
}
void DrinkOrder::save(std::shared_ptr<DrinkOrder> order){
    DrinkOrderGateway gateway;
    gateway.save(order);
}

std::shared_ptr<DrinkOrder> DrinkOrder::create(
            std::shared_ptr<Drink> drink,
            std::shared_ptr<Employeer> waiter,
            int table
    ){
    DrinkOrderGateway gateway;
    return gateway.create(drink->get_id(), waiter->get_id(),table);
}

void DrinkOrder::remove(std::shared_ptr<DrinkOrder> order){
    DrinkOrderGateway gateway;
    gateway.remove(order);
}
