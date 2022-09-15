//
// Created by victor on 08.09.2022.
//

#include "Order.h"

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
