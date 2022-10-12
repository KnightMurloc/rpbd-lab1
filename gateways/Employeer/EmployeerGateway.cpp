//
// Created by victor on 07.09.2022.
//

#include "EmployeerGateway.h"
#include "../DbInstance.h"
#include "Employeer.h"
#include <iostream>
#include <fmt/format.h>
#include <memory>
#include <string>
#include "../Orders/OrderGateway.h"

// template<>
// lru_cache_t<int,std::shared_ptr<Employeer>> IGateway<Employeer>::cache(CACHE_SIZE);

// template<>
// cache<Employeer> IGateway<Employeer>::cache_new;

std::shared_ptr<Employeer> EmployeerGateway::get(int id) {
//     auto result = cache.TryGet(id);
//     if(result.second){
//         return (*result.first).second;
//     }else{
//         auto db = DbInstance::getInstance();
//         std::string sql = fmt::format("select * from employees where id = {}", id);
//         auto response = db.exec(sql);
//         if(response.next()){
//             Employeer employeer(response.get<int>(0));
//             employeer.setFirstName(response.get<std::string>(1));
//             employeer.setLastName(response.get<std::string>(2));
//             if(!response.is_null(3)) {
//                 employeer.setPatronymic(response.get<std::string>(3));
//             }
//             employeer.setAddress(response.get<std::string>(4));
//             employeer.setBirthDate(response.get<std::string>(5));
//             employeer.setSalary(response.get<float>(6));
//             if(!response.is_null(7)) {
//                 employeer.set_movement_id(response.get<int>(7));
//             }
//             employeer.setPost(string_to_post(response.get<std::string>(8)));
//
//             auto ptr = std::make_shared<Employeer>(employeer);
//             cache.Put(id, ptr);
//             return ptr;
//         }
//     }

    if(cache_new.exist(id)){
        return cache_new.get(id);
    }else{
        auto db = DbInstance::getInstance();
        std::string sql = fmt::format("select id, first_name, last_name, patronymic, address, birth_date, salary, movement, post from employees where id = {}", id);
        auto response = db.exec(sql);
        if(response.next()){

            OrderGateway gateway;

            Employeer employeer(response.get<int>(0));
            employeer.setFirstName(response.get<std::string>(1));
            employeer.setLastName(response.get<std::string>(2));
            if(!response.is_null(3)) {
                employeer.setPatronymic(response.get<std::string>(3));
            }
            employeer.setAddress(response.get<std::string>(4));
            employeer.setBirthDate(response.get<std::string>(5));
            employeer.setSalary(response.get<float>(6));
            if(!response.is_null(7)) {
//                 employeer.set_movement_id(response.get<int>(7));
                employeer.set_movement(gateway.get(response.get<int>(7)));
            }
            employeer.setPost(string_to_post(response.get<std::string>(8)));

            auto ptr = std::make_shared<Employeer>(employeer);
//             cache.Put(id, ptr);
            cache_new.put(ptr);
            return ptr;
        }
    }

    throw GatewayException("not found");
}

std::list<std::shared_ptr<Employeer>> EmployeerGateway::get_all() {

    std::list<std::shared_ptr<Employeer>> result;

    auto db = DbInstance::getInstance();

    auto response = db.exec("select id from employees;");

    while(response.next()){
//         Employeer employeer(response.get<int>(0));
//         employeer.setFirstName(response.get<std::string>(1));
//         employeer.setLastName(response.get<std::string>(2));
//         if(!response.is_null(3)) {
//             employeer.setPatronymic(response.get<std::string>(3));
//         }
//         employeer.setAddress(response.get<std::string>(4));
//         employeer.setBirthDate(response.get<std::string>(5));
//         employeer.setSalary(response.get<float>(6));
//         if(!response.is_null(7)) {
//             employeer.set_movement_id(response.get<int>(7));
//         }
//         employeer.setPost(string_to_post(response.get<std::string>(8)));
//
//         auto ptr = std::make_shared<Employeer>(employeer);
// //         cache.Put(ptr->get_id(), ptr);
//
//         result.push_back(ptr);

        result.push_back(get(response.get<int>(0)));
    }

    return result;
}

void EmployeerGateway::save(std::shared_ptr<Employeer> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("update employees set"
                                  " first_name = '{}',"
                                  " last_name = '{}',"
                                  " patronymic = '{}',"
                                  " address = '{}',"
                                  " birth_date = '{}',"
                                  " salary = {},"
                                  " movement = {},"
                                  " post = '{}'"
                                  " where id = {};",
                                  data->getFirstName(),
                                  data->getLastName(),
                                  data->getPatronymic(),
                                  data->getAddress(),
                                  data->getBirthDate(),
                                  data->getSalary(),
                                  data->get_movement() == nullptr ? "NULL" : std::to_string(data->get_movement()->get_id()),
                                  post_to_string(data->getPost()),
                                  data->get_id()
                                  );

    db.exec(sql);
}

std::shared_ptr<Employeer>
EmployeerGateway::create(std::string first_name, std::string last_name, std::string patronymic, std::string address,
                         std::string birth_date, float salary, int movement_id, Post post) {
    auto db = DbInstance::getInstance();
    std::string sql = fmt::format("insert into employees(first_name,last_name,patronymic,address,birth_date,salary,movement,post)"
                                  " values('{}','{}','{}', '{}', '{}', {}, {}, '{}')"
                                  " returning id;",
                                  first_name,
                                  last_name,
                                  patronymic,
                                  address,
                                  birth_date,
                                  salary,
                                  movement_id == -1 ? "NULL" : std::to_string(movement_id),
                                  post_to_string(post)
                                  );
    auto result = db.exec(sql);

    if(result.next()){
        OrderGateway gateway;

        Employeer empl(result.get<int>(0));
        empl.setFirstName(first_name);
        empl.setLastName(last_name);
        empl.setPatronymic(patronymic);
        empl.setAddress(address);
        empl.setBirthDate(birth_date);
        empl.setSalary(salary);
//         empl.set_movement_id(movement_id);
        empl.set_movement(gateway.get(movement_id));
        empl.setPost(post);

        auto ptr = std::make_shared<Employeer>(empl);

//         cache.Put(ptr->get_id(), ptr);
        cache_new.put(ptr);

        return ptr;
    }

    throw GatewayException("create error");
}

void EmployeerGateway::remove(std::shared_ptr<Employeer> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from employees where id = {};", data->get_id());

    db.exec(sql);

//     cache.Remove(data->get_id());
    cache_new.remove(data->get_id());
}

std::list<std::shared_ptr<Employeer>> EmployeerGateway::get_great_then_by_id(int id, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from employees where id > {} order by id limit {}",id,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Employeer>> result;

    while(response.next()){
//         auto empl = std::make_shared<Employeer>(response.get<int>(0));
//         empl->setFirstName(response.get<std::string>(1));
//         empl->setLastName(response.get<std::string>(2));
//         if(!response.is_null(3)) {
//             empl->setPatronymic(response.get<std::string>(3));
//         }
//         empl->setAddress(response.get<std::string>(4));
//         empl->setBirthDate(response.get<std::string>(5));
//         empl->setSalary(response.get<float>(6));
//         if(!response.is_null(7)) {
//             empl->set_movement_id(response.get<int>(7));
//         }
//         empl->setPost(string_to_post(response.get<std::string>(8)));
// //         cache.Put(empl->get_id(), empl);
//         cache_new.put(empl);
//         result.push_back(empl);


        result.push_back(get(response.get<int>(0)));
    }
    return result;
}

std::list<std::shared_ptr<Employeer>> EmployeerGateway::get_less_then_by_id(int id, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from employees where id < {} order by id DESC limit {};",id,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Employeer>> result;

    while(response.next()){
//         auto empl = std::make_shared<Employeer>(response.get<int>(0));
//         empl->setFirstName(response.get<std::string>(1));
//         empl->setLastName(response.get<std::string>(2));
//         if(!response.is_null(3)) {
//             empl->setPatronymic(response.get<std::string>(3));
//         }
//         empl->setAddress(response.get<std::string>(4));
//         empl->setBirthDate(response.get<std::string>(5));
//         empl->setSalary(response.get<float>(6));
//         if(!response.is_null(7)) {
//             empl->set_movement_id(response.get<int>(7));
//         }
//         empl->setPost(string_to_post(response.get<std::string>(8)));
// //         cache.Put(empl->get_id(), empl);
//         cache_new.put(empl);
//         result.push_back(empl);
        result.push_back(get(response.get<int>(0)));
    }
    return result;
}

std::list<std::shared_ptr<Employeer>> EmployeerGateway::get_great_then_by_name(std::string name, int id, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format(
        "select id from employees where ("
        "    lower(first_name) like '%{0}%'"
        " or lower(last_name) like '%{0}%'"
        " or lower(patronymic) like '%{0}%'"
        " ) and id > {1} order by id DESC limit {2};", name,id,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Employeer>> result;

    while(response.next()){
//         auto empl = std::make_shared<Employeer>(response.get<int>(0));
//         empl->setFirstName(response.get<std::string>(1));
//         empl->setLastName(response.get<std::string>(2));
//         if(!response.is_null(3)) {
//             empl->setPatronymic(response.get<std::string>(3));
//         }
//         empl->setAddress(response.get<std::string>(4));
//         empl->setBirthDate(response.get<std::string>(5));
//         empl->setSalary(response.get<float>(6));
//         if(!response.is_null(7)) {
//             empl->set_movement_id(response.get<int>(7));
//         }
//         empl->setPost(string_to_post(response.get<std::string>(8)));
// //         cache.Put(empl->get_id(), empl);
//         cache_new.put(empl);
//         result.push_back(empl);
        result.push_back(get(response.get<int>(0)));
    }
    return result;
}

std::list<std::shared_ptr<Employeer>> EmployeerGateway::get_less_then_by_name(std::string name, int id, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format(
        "select id from employees where ("
        "    lower(first_name) like '%{0}%'"
        " or lower(last_name) like '%{0}%'"
        " or lower(patronymic) like '%{0}%'"
        " ) and id < {1} order by id DESC limit {2};", name,id,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Employeer>> result;

    while(response.next()){
//         auto empl = std::make_shared<Employeer>(response.get<int>(0));
//         empl->setFirstName(response.get<std::string>(1));
//         empl->setLastName(response.get<std::string>(2));
//         if(!response.is_null(3)) {
//             empl->setPatronymic(response.get<std::string>(3));
//         }
//         empl->setAddress(response.get<std::string>(4));
//         empl->setBirthDate(response.get<std::string>(5));
//         empl->setSalary(response.get<float>(6));
//         if(!response.is_null(7)) {
//             empl->set_movement_id(response.get<int>(7));
//         }
//         empl->setPost(string_to_post(response.get<std::string>(8)));
// //         cache.Put(empl->get_id(), empl);
//         cache_new.put(empl);
//         result.push_back(empl);
        result.push_back(get(response.get<int>(0)));
    }
    return result;
}
