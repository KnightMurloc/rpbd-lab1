//
// Created by victor on 30.09.2022.
//

#include "SnackOrder.h"
#include "../Snacks/SnackGateway.h"
#include "../Drinks/Drinkgateway.h"
#include "../Employeer/EmployeerGateway.h"
#include "SnackOrdergateway.h"

int SnackOrder::get_id() const {
    return id;
}

SnackOrder::SnackOrder(int id) : id(id) {}

// int SnackOrder::get_snack_id() const {
//     return snack;
// }
//
// int SnackOrder::get_waiter_id() const {
//     return waiter;
// }

int SnackOrder::get_table() const {
    return table_;
}

// void SnackOrder::set_snack_id(int snack) {
//     SnackOrder::snack = snack;
// }
//
// void SnackOrder::set_waiter_id(int waiter) {
//     SnackOrder::waiter = waiter;
// }

void SnackOrder::set_table(int table) {
    table_ = table;
}

std::shared_ptr<Snack> SnackOrder::get_snack(){
//     SnackGateway gateway;
//     return gateway.get(snack);
    return snack;
}

std::shared_ptr<Employeer> SnackOrder::get_waiter(){
//     EmployeerGateway gateway;
//     return gateway.get(waiter);
    return waiter;
}

void SnackOrder::set_snack(std::shared_ptr<Snack> snack){
    this->snack = snack;
}

void SnackOrder::set_waiter(std::shared_ptr<Employeer> waiter){
    this->waiter = waiter;
}

std::list<std::shared_ptr<SnackOrder>> SnackOrder::get_great_than_by_id(int id, int count) {
    SnackOrdergateway gateway;
    return gateway.get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<SnackOrder>> SnackOrder::get_less_than_by_id(int id, int count) {
    SnackOrdergateway gateway;
    return gateway.get_less_then_by_id(id,count);
}

void SnackOrder::save(std::shared_ptr<SnackOrder> order){
    SnackOrdergateway gateway;
    gateway.save(order);
}

std::shared_ptr<SnackOrder> SnackOrder::create(
        int snack_id,
        int waiter_id,
        int table
    ){
        SnackOrdergateway gateway;
        return gateway.create(snack_id,waiter_id,table);
}

void SnackOrder::remove(std::shared_ptr<SnackOrder> order){
    SnackOrdergateway gateway;
    gateway.remove(order);
}
