//
// Created by victor on 30.09.2022.
//

#include "SnackOrder.h"
#include "../Snacks/SnackGateway.h"
#include "../Drinks/Drinkgateway.h"
#include "../Employeer/EmployeerGateway.h"

int SnackOrder::get_id() const {
    return id;
}

SnackOrder::SnackOrder(int id) : id(id) {}

int SnackOrder::get_snack_id() const {
    return snack;
}

int SnackOrder::get_waiter_id() const {
    return waiter;
}

int SnackOrder::get_table() const {
    return table_;
}

void SnackOrder::set_snack_id(int snack) {
    SnackOrder::snack = snack;
}

void SnackOrder::set_waiter_id(int waiter) {
    SnackOrder::waiter = waiter;
}

void SnackOrder::set_table(int table) {
    table_ = table;
}

std::shared_ptr<Snack> SnackOrder::get_snack(){
    SnackGateway gateway;
    return gateway.get(snack);
}

std::shared_ptr<Employeer> SnackOrder::get_waiter(){
    EmployeerGateway gateway;
    return gateway.get(waiter);
}
