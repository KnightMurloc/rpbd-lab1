//
// Created by victor on 15.09.2022.
//

#include "Drinkgateway.h"
#include "../DbInstance.h"
#include "Drink.h"
#include "fmt/core.h"
#include <fmt/format.h>
#include <memory>

// template<>
// lru_cache_t<int,std::shared_ptr<Drink>> IGateway<Drink>::cache(CACHE_SIZE);

// template<>
// cache<Drink> IGateway<Drink>::cache_new;

void Drinkgateway::save(std::shared_ptr<Drink> data) {
    auto db = DbInstance::getInstance();

    db.exec("begin transaction;");

    std::string sql = fmt::format("update drinks set name = '{}', strength = {}, size = {}, container = '{}' where id = {}",
        data->getName(),
        data->getStrength(),
        data->getSize(),
        data->getContainer(),
        data->get_id()
    );

    db.exec(sql);

    auto rm_ing = data->getRecipe().get_removed_ingridients();
    auto new_ing = data->getRecipe().get_new_ingridients();

    for(auto ing : rm_ing){
        sql = fmt::format("delete from drink_recipes where ingredient = {};",
            ing->get_id()
        );
        db.exec(sql);
    }

    for(auto ing : new_ing){
        sql = fmt::format("insert into drink_recipes(ingredient, drink, count) values({},{},{});",
            std::get<0>(ing)->get_id(),
            data->get_id(),
            std::get<1>(ing)
        );
        db.exec(sql);
    }

    db.exec("commit;");
}

std::shared_ptr<Drink> Drinkgateway::get(int id) {
//     auto result = cache.TryGet(id);
//     if(result.second){
//         return (*result.first).second;
//     }else{
//         auto db = DbInstance::getInstance();
//         std::string sql = fmt::format("select * from drinks where id = {};", id);
//
//         auto response = db.exec(sql);
//
//         if(response.next()){
//             Drink drink(response.get<int>(0));
//
//             drink.setName(response.get<std::string>(1));
//             drink.setStrength(response.get<int>(2));
//             drink.setSize(response.get<int>(3));
//             drink.setContainer(response.get<std::string>(4));
//
//             auto ptr = std::make_shared<Drink>(drink);
//             cache.Put(id, ptr);
//
//             return ptr;
//         }
//     }

    if(cache_new.exist(id)){
        return cache_new.get(id);
    }else{
        auto db = DbInstance::getInstance();
        std::string sql = fmt::format("select id, name, strength, size, container  from drinks where id = {};", id);

        auto response = db.exec(sql);

        if(response.next()){
            Drink drink(response.get<int>(0));

            drink.setName(response.get<std::string>(1));
            drink.setStrength(response.get<int>(2));
            drink.setSize(response.get<int>(3));
            drink.setContainer(response.get<std::string>(4));

            auto ptr = std::make_shared<Drink>(drink);
            cache_new.put(ptr);

            return ptr;
        }
    }

    throw GatewayException("not found");
}

void Drinkgateway::remove(std::shared_ptr<Drink> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from drinks where id = {};", data->get_id());
    db.exec(sql);
//     cache.Remove(data->get_id());
    cache_new.remove(data->get_id());
}

std::list<std::shared_ptr<Drink>> Drinkgateway::get_all() {
    auto db = DbInstance::getInstance();

    std::list<std::shared_ptr<Drink>> result;

    auto response = db.exec("select id from drinks;");

    while(response.next()){
//         Drink drink(response.get<int>(0));
//
//         drink.setName(response.get<std::string>(1));
//         drink.setStrength(response.get<int>(2));
//         drink.setSize(response.get<int>(3));
//         drink.setContainer(response.get<std::string>(4));

//         result.push_back(std::make_shared<Drink>(drink));
        result.push_back(get(response.get<int>(0)));
    }

    return result;
}

std::list<std::pair<int,int>> Drinkgateway::get_ingredients(std::shared_ptr<Drink> data){
    auto db = DbInstance::getInstance();


    std::string sql = fmt::format("select ingredient,count from drink_recipes where drink = {};",
       data->get_id()
    );

    auto response = db.exec(sql);

    std::list<std::pair<int,int>> result;

    while(response.next()){
        result.push_back(std::make_pair(response.get<int>(0),response.get<int>(1)));
    }
    return result;
}

std::shared_ptr<Drink> Drinkgateway::create(std::string name, int strength, int size, std::string container,std::vector<std::pair<int,int>> ings){
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

    std::string sql = fmt::format("insert into drinks(name, strength, size, container)"
        "values('{}', {}, {}, '{}') returning id;",
        name,
        strength,
        size,
        container
    );

    auto response = db.exec(sql);
    if(response.next()){
        Drink drink(response.get<int>(0));
        drink.setName(name);
        drink.setStrength(strength);
        drink.setSize(size);
        drink.setContainer(container);

        for(auto ing : ings){
            sql = fmt::format("insert into drink_recipes(ingredient, drink, count) values({},{},{});",
                ing.first,
                drink.get_id(),
                ing.second
            );
            db.exec(sql);
        }
        db.exec("commit;");
        auto ptr = std::make_shared<Drink>(drink);

        cache_new.put(ptr);

        return ptr;
    }
    db.exec("rollback;");

    throw GatewayException("create error");
}

std::list<std::shared_ptr<Drink>> Drinkgateway::get_great_then_by_id(int min, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from drinks where id > {} order by id limit {}",min,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Drink>> result;

    while(response.next()){
//         auto drink = std::make_shared<Drink>(response.get<int>(0));
//         drink->setName(response.get<std::string>(1));
//         drink->setStrength(response.get<int>(2));
//         drink->setSize(response.get<int>(3));
//         drink->setContainer(response.get<std::string>(4));
//
// //         cache.Put(drink->get_id(), drink);
//         cache_new.put(drink);
//         result.push_back(drink);

        result.push_back(get(response.get<int>(0)));
    }
    return result;
}

std::list<std::shared_ptr<Drink>> Drinkgateway::get_less_then_by_id(int min, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from drinks where id < {} order by id DESC limit {};",min,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Drink>> result;

    while(response.next()){
//         auto drink = std::make_shared<Drink>(response.get<int>(0));
//         drink->setName(response.get<std::string>(1));
//         drink->setStrength(response.get<int>(2));
//         drink->setSize(response.get<int>(3));
//         drink->setContainer(response.get<std::string>(4));
//
// //         cache.Put(drink->get_id(), drink);
//         cache_new.put(drink);
//         result.push_back(drink);
        result.push_back(get(response.get<int>(0)));
    }
    return result;
}
