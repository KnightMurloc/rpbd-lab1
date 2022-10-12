//
// Created by victor on 08.09.2022.
//

#include "Order.h"
#include "OrderGateway.h"

Order::Order(int id) : id(id) {}

int Order::get_id() const {
    return id;
}

const std::string &Order::get_reason() const {
    return reason;
}

void Order::set_reason(const std::string &reason) {
    Order::reason = reason;
}

int Order::get_order_number() const {
    return order_number;
}

void Order::set_order_number(int orderNumber) {
    order_number = orderNumber;
}

const std::string &Order::get_order_date() const {
    return order_date;
}

void Order::set_order_date(const std::string &orderDate) {
    order_date = orderDate;
}

Post Order::get_post() const {
    return post;
}

void Order::set_post(Post post) {
    Order::post = post;
}

std::string Order::get_post_as_string() const {
    return post_to_string(post);
}

std::shared_ptr<Order> Order::get(int id){
    OrderGateway gateway;
    return gateway.get(id);
}

std::list<std::shared_ptr<Order>> Order::get_great_than_by_id(int id, int count) {
    OrderGateway gateway;
    return gateway.get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<Order>> Order::get_less_than_by_id(int id, int count) {
    OrderGateway gateway;
    return gateway.get_less_then_by_id(id,count);
}

void Order::save(std::shared_ptr<Order> order){
    OrderGateway gateway;
    gateway.save(order);
}
std::shared_ptr<Order> Order::create(const std::string &reason, int order_number, const std::string &order_date, const std::string& post){
    OrderGateway gateway;
    return gateway.create(reason,order_number,order_date,post);
}

void Order::remove(std::shared_ptr<Order> order){
    OrderGateway gateway;
    gateway.remove(order);
}
