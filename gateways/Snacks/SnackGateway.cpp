//
// Created by victor on 11.09.2022.
//

#include "SnackGateway.h"
#include "../DbInstance.h"
#include "Snacks.h"
#include "fmt/core.h"
#include <fmt/format.h>
#include <utility>

// template<>
// lru_cache_t<int,std::shared_ptr<Snack>> IGateway<Snack>::cache(CACHE_SIZE);

// template<>
// cache<Snack> IGateway<Snack>::cache_new;

void SnackGateway::save(std::shared_ptr<Snack> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("update snacks set name = '{}', size = {} where id = {}",
        escape_string(data->get_name()),
        data->get_size(),
        data->get_id()
    );

    db.exec(sql);

    auto rm_ing = data->get_recipe().get_removed_ingridients();
    auto new_ing = data->get_recipe().get_new_ingridients();

    for(auto ing : rm_ing){
        sql = fmt::format("delete from snack_recipes where ingredient = {};",
            ing->get_id()
        );
        db.exec(sql);
    }

    for(auto ing : new_ing){
        sql = fmt::format("insert into snack_recipes(ingredient,snack , count) values({},{},{});",
            std::get<0>(ing)->get_id(),
            data->get_id(),
            std::get<1>(ing)
        );
        db.exec(sql);
    }
}

std::shared_ptr<Snack> SnackGateway::create(std::string name, int size, std::vector<std::pair<int,int>> ings){
    auto db = DbInstance::getInstance();

    db.exec("begin transaction;");
//     std::string sql = "insert into recipes(id) values (default) returning id;";
//     auto response = db.exec(sql);
//     int r_id;
//     if(response.next()){
//         r_id = response.get<int>(0);
//     }else{
//         db.exec("rollback;");
//         throw GatewayException("create error");
//     }

    std::string sql = fmt::format("insert into snacks(name, size) values('{}',{}) returning id;",escape_string(name),size);

    auto response = db.exec(sql);

    if(response.next()){
        Snack snack(response.get<int>(0));
        snack.set_name(name);
        snack.set_size(size);

        for(auto ing : ings){
            sql = fmt::format("insert into snack_recipes(ingredient, snack, count) values({},{},{});",
                ing.first,
                snack.get_id(),
                ing.second
            );
            db.exec(sql);
        }

        auto ptr = std::make_shared<Snack>(snack);

//         cache.Put(ptr->get_id(), ptr);
        cache_new.put(ptr);

        db.exec("commit;");
        return ptr;
    }
    db.exec("rollback;");
    throw GatewayException("create error");
}

std::shared_ptr<Snack> SnackGateway::get(int id) {
//     auto result = cache.TryGet(id);
//     if(result.second){
//         return (*result.first).second;
//     }else{
//         auto db = DbInstance::getInstance();
//
//         std::string sql = fmt::format("select * from snacks where id = {};", id);
//
//         auto response = db.exec(sql);
//
//         if(response.next()){
//             Snack snack(response.get<int>(0));
//             snack.set_name(response.get<std::string>(1));
//             snack.set_size(response.get<int>(2));
//
//             auto ptr = std::make_shared<Snack>(snack);
//             cache.Put(id, ptr);
//
//             return ptr;
//         }
//     }

    if(cache_new.exist(id)){
        return cache_new.get(id);
    }else{
        auto db = DbInstance::getInstance();

        std::string sql = fmt::format("select id, name, size from snacks where id = {};", id);

        auto response = db.exec(sql);

        if(response.next()){
            Snack snack(response.get<int>(0));
            snack.set_name(response.get<std::string>(1));
            snack.set_size(response.get<int>(2));

            auto ptr = std::make_shared<Snack>(snack);
//             cache.Put(id, ptr);
            cache_new.put(ptr);

            return ptr;
        }
    }

    throw GatewayException("not found");
}

void SnackGateway::remove(std::shared_ptr<Snack> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from snacks where id = {};",
            data->get_id());

    db.exec(sql);

//     cache.Remove(data->get_id());
    cache_new.remove(data->get_id());
}

std::list<std::shared_ptr<Snack>> SnackGateway::get_all() {

    auto db = DbInstance::getInstance();

    std::string sql = "select id from snacks;";

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Snack>> result;

    while(response.next()){
//         Snack snack(response.get<int>(0));
//         snack.set_name(response.get<std::string>(1));
//         snack.set_size(response.get<int>(2));
//
//         auto ptr = std::make_shared<Snack>(snack);
//
// //         cache.Put(ptr->get_id(),ptr);
//         cache_new.put(ptr);
//
//         result.push_back(ptr);
        result.push_back(get(response.get<int>(0)));
    }

    return result;
}

std::list<std::pair<int,int>> SnackGateway::get_ingredients(std::shared_ptr<Snack> data){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select ingredient, count from snack_recipes where snack = {};",
        data->get_id()
    );

    auto response = db.exec(sql);

    std::list<std::pair<int,int>> result;

    while(response.next()){
        result.push_back(std::make_pair(response.get<int>(0),response.get<int>(1)));
    }

    return result;
}

std::list<std::shared_ptr<Snack>> SnackGateway::get_great_then_by_id(int min, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from snacks where id > {} order by id limit {}",min,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Snack>> result;

    while(response.next()){
//         auto snack = std::make_shared<Snack>(response.get<int>(0));
//         snack->set_name(response.get<std::string>(1));
//         snack->set_size(response.get<int>(2));
// //         cache.Put(snack->get_id(), snack);
//         cache_new.put(snack);
//         result.push_back(snack);
        result.push_back(get(response.get<int>(0)));
    }
    return result;
}

std::list<std::shared_ptr<Snack>> SnackGateway::get_less_then_by_id(int min, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from snacks where id < {} order by id DESC limit {};",min,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Snack>> result;

    while(response.next()){
//         auto snack = std::make_shared<Snack>(response.get<int>(0));
//         snack->set_name(response.get<std::string>(1));
//         snack->set_size(response.get<int>(2));
// //         cache.Put(snack->get_id(), snack);
//         cache_new.put(snack);
//         result.push_back(snack);
        result.push_back(get(response.get<int>(0)));
    }
    return result;
}
