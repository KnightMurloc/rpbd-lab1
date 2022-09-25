//
// Created by victor on 08.09.2022.
//

#include "OrderGateway.h"
#include "../DbInstance.h"
#include "Order.h"
#include <fmt/format.h>
#include <memory>

template<>
lru_cache_t<int,std::shared_ptr<Order>> IGateway<Order>::cache(CACHE_SIZE);

std::shared_ptr<Order> OrderGateway::get(int id) {

    auto result = cache.TryGet(id);
    if(result.second){
        return (*result.first).second;
    }else{

        auto db = DbInstance::getInstance();

        auto response = db.exec("select * from orders where id = " + std::to_string(id));

        if(response.next()){
            Order order(response.get<int>(0));
            order.set_reason(response.get<std::string>(1));
            order.set_order_number(response.get<int>(2));
            order.set_post(string_to_post(response.get<std::string>(3)));
            order.set_order_date(response.get<std::string>(4));

            auto ptr = std::make_shared<Order>(order);
            cache.Put(id, ptr);

            return ptr;
        }
    }
    throw GatewayException("not found");
}

std::list<std::shared_ptr<Order>> OrderGateway::get_all() {
    std::list<std::shared_ptr<Order>> result;

    auto db = DbInstance::getInstance();

    auto response = db.exec("select * from orders;");

    while(response.next()){
        Order order(response.get<int>(0));
        order.set_reason(response.get<std::string>(1));
        order.set_order_number(response.get<int>(2));
        order.set_post(string_to_post(response.get<std::string>(3)));
        order.set_order_date(response.get<std::string>(4));

        auto ptr = std::make_shared<Order>(order);

        cache.Put(ptr->get_id(), ptr);

        result.push_back(ptr);
    }

    return result;
}

std::shared_ptr<Order> OrderGateway::create(const std::string &reason, int order_number, const std::string &order_date, const std::string& post) {
    auto db = DbInstance::getInstance();

    auto response =
            db.exec(fmt::format("insert into orders(reason, order_number, order_date, post)"
                                "values('{}', {}, '{}', '{}') returning id;",
                                reason,
                                order_number,
                                order_date,
                                post));

    if(response.next()){
        Order order(response.get<int>(0));
        order.set_reason(reason);
        order.set_order_number(order_number);
        order.set_order_date(order_date);
        order.set_post(string_to_post(post));

        auto ptr = std::make_shared<Order>(order);

        cache.Put(ptr->get_id(),ptr);

        return ptr;
    }

    throw GatewayException("create error");
}

void OrderGateway::save(std::shared_ptr<Order> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("update orders set"
                                  " reason = '{}',"
                                  " order_number = {},"
                                  " order_date = '{}',"
                                  " post = '{}'"
                                  " where id = {};",
                                  data->get_reason(),
                                  data->get_order_number(),
                                  data->get_order_date(),
                                  data->get_post_as_string(),
                                  data->get_id());
    db.exec(sql);
}

void OrderGateway::remove(std::shared_ptr<Order> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from orders where id = {};", data->get_id());

    cache.Remove(data->get_id());

    db.exec(sql);
}
