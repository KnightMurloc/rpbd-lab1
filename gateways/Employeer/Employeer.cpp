//
// Created by victor on 07.09.2022.
//

#include "Employeer.h"
#include "../Orders/OrderGateway.h"

int Employeer::getId() const {
    return id;
}

const std::string &Employeer::getFirstName() const {
    return first_name;
}

void Employeer::setFirstName(const std::string &firstName) {
    first_name = firstName;
}

const std::string &Employeer::getLastName() const {
    return last_name;
}

void Employeer::setLastName(const std::string &lastName) {
    last_name = lastName;
}

const std::string &Employeer::getPatronymic() const {
    return patronymic;
}

void Employeer::setPatronymic(const std::string &patronymic) {
    Employeer::patronymic = patronymic;
}

const std::string &Employeer::getAddress() const {
    return address;
}

void Employeer::setAddress(const std::string &address) {
    Employeer::address = address;
}

std::string Employeer::getBirthDate() const {
    return birth_date;
}

void Employeer::setBirthDate(std::string birthDate) {
    birth_date = birthDate;
}

float Employeer::getSalary() const {
    return salary;
}

void Employeer::setSalary(float salary) {
    Employeer::salary = salary;
}

Post Employeer::getPost() const {
    return post;
}

void Employeer::setPost(Post post) {
    Employeer::post = post;
}

std::string Employeer::getPostAsString() const {
    if(post == Post::waiter){
        return "waiter";
    }
    if(post == Post::manager){
        return "manager";
    }
    if(post == Post::bartender){
        return "bartender";
    }
    return std::string();
}

Employeer::Employeer(int id) : id(id) {}

int Employeer::get_movement_id() const {
    return movement_id;
}

void Employeer::set_movement_id(int movementId) {
    movement_id = movementId;
}

Order Employeer::get_movement() {
    if(movement_id == -1){
        throw GatewayException("not found");
    }
    OrderGateway gateway;

    try {
        auto order = gateway.get(movement_id);
        return order;
    } catch (GatewayException& exception) {
        movement_id = -1;
        throw exception;
    }
}
