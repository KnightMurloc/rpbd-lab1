//
// Created by victor on 02.10.2022.
//

#include "DrinkOrderGateway.h"
#include "../DbInstance.h"
#include <fmt/format.h>

#include "../Drinks/Drinkgateway.h"
#include "../Employeer/EmployeerGateway.h"

// template<>
// lru_cache_t<int,std::shared_ptr<DrinkOrder>> IGateway<DrinkOrder>::cache(CACHE_SIZE);

// template<>
// cache<DrinkOrder> IGateway<DrinkOrder>::cache_new;

std::shared_ptr<DrinkOrder> DrinkOrderGateway::create(
        int drink_id,
        int waiter_id,
        int table
){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format(
            "insert into drink_orders(drink, waiter, table_) values({},{},{}) returning id;",
            drink_id,
            waiter_id,
            table
    );

    auto response = db.exec(sql);
    if(response.next()){

        EmployeerGateway waiter_gateway;
        Drinkgateway drink_gateway;

        auto order = std::make_shared<DrinkOrder>(response.get<int>(0));

        order->set_drink(drink_gateway.get(drink_id));
        order->set_waiter(waiter_gateway.get(waiter_id));
//         order->set_drink_id(drink_id);
//         order->set_waiter_id(waiter_id);

        order->set_table(table);

//         cache.Put(order->get_id(), order);
        cache_new.put(order);
        return order;
    }
    throw GatewayException("create error");
}

void DrinkOrderGateway::save(std::shared_ptr<DrinkOrder> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format(
            "update drink_orders set "
            " drink = {},"
            " waiter = {},"
            " table_ = {}"
            " where id = {};",
            data->get_drink()->get_id(),
            data->get_waiter()->get_id(),
            data->get_table(),
            data->get_id()
    );

    db.exec(sql);
}

std::shared_ptr<DrinkOrder> DrinkOrderGateway::get(int id) {

    if(cache_new.exist(id)){
        return cache_new.get(id);
    }else{
        auto db = DbInstance::getInstance();

        std::string sql = fmt::format("select id,drink,waiter,table_ from drink_orders");

        auto response = db.exec(sql);

        if(response.next()){
            auto order = std::make_shared<DrinkOrder>(response.get<int>(0));

            EmployeerGateway waiter_gateway;
            Drinkgateway drink_gateway;

//             order->set_drink_id(response.get<int>(1));
//             order->set_waiter_id(response.get<int>(2));
            order->set_drink(drink_gateway.get(response.get<int>(1)));
            order->set_waiter(waiter_gateway.get(response.get<int>(2)));
            order->set_table(response.get<int>(3));

            cache_new.put(order);

            return order;
        }
    }

    return std::shared_ptr<DrinkOrder>();
}

void DrinkOrderGateway::remove(std::shared_ptr<DrinkOrder> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from drink_orders where id = {};",data->get_id());

    db.exec(sql);

//     cache.Remove(data->get_id());
    cache_new.remove(data->get_id());
}

std::list<std::shared_ptr<DrinkOrder>> DrinkOrderGateway::get_all() {
    return std::list<std::shared_ptr<DrinkOrder>>();
}

std::list<std::shared_ptr<DrinkOrder>> DrinkOrderGateway::get_great_then_by_id(int id, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from drink_orders where id > {} order by id limit {}",id,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<DrinkOrder>> result;

    while(response.next()){
//         auto order = std::make_shared<DrinkOrder>(response.get<int>(0));
//
//         order->set_drink_id(response.get<int>(1));
//         order->set_waiter_id(response.get<int>(2));
//         order->set_table(response.get<int>(3));
//
// //         cache.Put(order->get_id(), order);
//         cache_new.put(order);
//         result.push_back(order);
        result.push_back(get(response.get<int>(0)));
    }
    return result;
}

std::list<std::shared_ptr<DrinkOrder>> DrinkOrderGateway::get_less_then_by_id(int id, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from drink_orders where id < {} order by id DESC limit {};",id,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<DrinkOrder>> result;

    while(response.next()){
//         auto order = std::make_shared<DrinkOrder>(response.get<int>(0));
//
//         order->set_drink_id(response.get<int>(1));
//         order->set_waiter_id(response.get<int>(2));
//         order->set_table(response.get<int>(3));
//
// //         cache.Put(order->get_id(), order);
//         DrinkOrderGateway::cache_new.put(order);
//         result.push_back(order);
        result.push_back(get(response.get<int>(0)));
    }
    return result;
}
