//
// Created by victor on 02.10.2022.
//

#include "DrinkOrder.h"
#include "../Drinks/Drinkgateway.h"
#include "../Employeer/EmployeerGateway.h"

int DrinkOrder::get_id() const {
    return id;
}

DrinkOrder::DrinkOrder(int id) : id(id) {}

int DrinkOrder::get_drink_id() const {
    return drink;
}

int DrinkOrder::get_waiter_id() const {
    return waiter;
}

int DrinkOrder::get_table() const {
    return table;
}

void DrinkOrder::set_drink_id(int drink) {
    DrinkOrder::drink = drink;
}

void DrinkOrder::set_waiter_id(int waiter) {
    DrinkOrder::waiter = waiter;
}

void DrinkOrder::set_table(int table) {
    DrinkOrder::table = table;
}

std::shared_ptr<Drink> DrinkOrder::get_drink() {
    Drinkgateway gateway;
    return gateway.get(drink);
}

std::shared_ptr<Employeer> DrinkOrder::get_waiter() {
    EmployeerGateway gateway;
    return gateway.get(waiter);
}
