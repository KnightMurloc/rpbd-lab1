//
// Created by victor on 30.09.2022.
//

#include "SnackOrdergateway.h"
#include "../DbInstance.h"
#include <fmt/format.h>
#include "../Snacks/SnackGateway.h"
#include "../Employeer/EmployeerGateway.h"

// template<>
// lru_cache_t<int,std::shared_ptr<SnackOrder>> IGateway<SnackOrder>::cache(CACHE_SIZE);

// template<>
// cache<SnackOrder> IGateway<SnackOrder>::cache_new;

std::shared_ptr<SnackOrder> SnackOrdergateway::create(
        int snack_id,
        int waiter_id,
        int table
    ){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format(
        "insert into snack_orders(snack, waiter, table_) values({},{},{}) returning id;",
        snack_id,
        waiter_id,
        table
    );

    auto response = db.exec(sql);
    if(response.next()){
        EmployeerGateway empl_gateway;
        SnackGateway snack_gatewaty;
        auto order = std::make_shared<SnackOrder>(response.get<int>(0));

        order->set_snack(snack_gatewaty.get(snack_id));
        order->set_waiter(empl_gateway.get(waiter_id));
        order->set_table(table);

//         cache.Put(order->get_id(), order);
        cache_new.put(order);
        return order;
    }
    throw GatewayException("create error");
}

void SnackOrdergateway::save(std::shared_ptr<SnackOrder> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format(
        "update snack_orders set "
        " snack = {},"
        " waiter = {},"
        " table_ = {}"
        " where id = {};",
        data->get_snack()->get_id(),
        data->get_waiter()->get_id(),
        data->get_table(),
        data->get_id()
    );

    db.exec(sql);
}

std::shared_ptr<SnackOrder> SnackOrdergateway::get(int id) {
    if(cache_new.exist(id)){
        return cache_new.get(id);
    }else{
        auto db = DbInstance::getInstance();

        std::string sql = fmt::format("select id, snack, waiter, table_ from snack_orders where id = {}",id);

        auto response = db.exec(sql);
        if(response.next()){
            EmployeerGateway empl_gateway;
            SnackGateway snack_gatewaty;

            auto order = std::make_shared<SnackOrder>(response.get<int>(0));

//             order->set_snack(response.get<int>(1));
//             order->set_waiter(response.get<int>(2));
            order->set_snack(snack_gatewaty.get(response.get<int>(1)));
            order->set_waiter(empl_gateway.get(response.get<int>(2)));
            order->set_table(response.get<int>(3));

    //         cache.Put(order->get_id(), order);
            cache_new.put(order);

            return order;
        }
    }

    throw GatewayException("not found");
}

void SnackOrdergateway::remove(std::shared_ptr<SnackOrder> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from snack_orders where id = {};",data->get_id());

    db.exec(sql);

//     cache.Remove(data->get_id());
    cache_new.remove(data->get_id());
}

std::list<std::shared_ptr<SnackOrder>> SnackOrdergateway::get_all() {
    return std::list<std::shared_ptr<SnackOrder>>();
}

std::list<std::shared_ptr<SnackOrder>> SnackOrdergateway::get_great_then_by_id(int id, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from snack_orders where id > {} order by id limit {}",id,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<SnackOrder>> result;

    while(response.next()){
//         auto order = std::make_shared<SnackOrder>(response.get<int>(0));
//
//         order->set_snack_id(response.get<int>(1));
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

std::list<std::shared_ptr<SnackOrder>> SnackOrdergateway::get_less_then_by_id(int id, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select * from snack_orders where id < {} order by id DESC limit {};",id,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<SnackOrder>> result;

    while(response.next()){
//         auto order = std::make_shared<SnackOrder>(response.get<int>(0));
//
//         order->set_snack_id(response.get<int>(1));
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
