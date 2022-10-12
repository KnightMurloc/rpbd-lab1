//
// Created by victor on 07.09.2022.
//

#include "Employeer.h"
#include "../Orders/OrderGateway.h"
#include "EmployeerGateway.h"
#include <exception>

int Employeer::get_id() const {
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

// int Employeer::get_movement_id() const {
//     return movement_id;
// }

// void Employeer::set_movement_id(int movementId) {
//     movement_id = movementId;
// }

std::shared_ptr<Order> Employeer::get_movement() const {
//     if(movement_id == -1){
//         throw GatewayException("not found");
//         //throw std::exception();
//      //   throw "test";
//     }
//     OrderGateway gateway;
//
//     try {
//         auto order = gateway.get(movement_id);
//         return order;
//     } catch (GatewayException& exception) {
//         movement_id = -1;
//         throw exception;
//     }
    return movement.lock();
}

void Employeer::set_movement(std::shared_ptr<Order> movement){
    this->movement = movement;
}

std::shared_ptr<Employeer> Employeer::get(int id){
    EmployeerGateway gateway;
    return gateway.get(id);
}

std::list<std::shared_ptr<Employeer>> Employeer::get_great_than_by_id(int id, int count) {
    EmployeerGateway gateway;
    return gateway.get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<Employeer>> Employeer::get_less_than_by_id(int id, int count) {
    EmployeerGateway gateway;
    return gateway.get_less_then_by_id(id,count);
}

void Employeer::save(std::shared_ptr<Employeer> empl){
    EmployeerGateway gateway;
    gateway.save(empl);
}

std::shared_ptr<Employeer> Employeer::create(std::string first_name,
            std::string last_name,
            std::string patronymic,
            std::string address,
            std::string birth_date,
            float salary,
            std::shared_ptr<Order> movement,
            Post post){
    EmployeerGateway gateway;

    return gateway.create(
      first_name,
      last_name,
      patronymic,
      address,
      birth_date,
      salary,
      movement->get_id(),
      post
    );
}

void Employeer::remove(std::shared_ptr<Employeer> empl){
    EmployeerGateway gateway;
    gateway.remove(empl);
}

std::list<std::shared_ptr<Employeer>> Employeer::get_great_than_by_id_filtred_by_name(std::string name,int id, int count){
    EmployeerGateway gateway;
    return gateway.get_great_then_by_name(name,id,count);
}

std::list<std::shared_ptr<Employeer>> Employeer::get_less_than_by_id_filtred_by_name(std::string name,int id, int count){
    EmployeerGateway gateway;
    return gateway.get_less_then_by_name(name,id,count);
}
