//
// Created by victor on 11.09.2022.
//

#include "SnackGateway.h"
#include "../DbInstance.h"
#include "Snacks.h"
#include "fmt/core.h"
#include <fmt/format.h>
#include <utility>

template<>
lru_cache_t<int,std::shared_ptr<Snack>> IGateway<Snack>::cache(CACHE_SIZE);

void SnackGateway::save(Snack &data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("update snacks set name = '{}', size = {} where id = {}",
        data.get_name(),
        data.get_size(),
        data.get_id()
    );

    db.exec(sql);

    auto rm_ing = data.get_recipe().get_removed_ingridients();
    auto new_ing = data.get_recipe().get_new_ingridients();

    for(auto ing : rm_ing){
        sql = fmt::format("delete from recipe_to_ingredient where ingredient = {};",
            ing
        );
        db.exec(sql);
    }

    for(auto ing : new_ing){
        sql = fmt::format("insert into recipe_to_ingredient(recipe, ingredient, count) values({},{},{});",
            data.get_recipe().get_id(),
            std::get<0>(ing),
            std::get<1>(ing)
        );
        db.exec(sql);
    }
}

Snack SnackGateway::create(std::string name, int size, std::vector<std::pair<int,int>> ings){
    auto db = DbInstance::getInstance();

    db.exec("begin transaction;");
    std::string sql = "insert into recipes(id) values (default) returning id;";
    auto response = db.exec(sql);
    int r_id;
    if(response.next()){
        r_id = response.get<int>(0);
    }else{
        return Snack(0,0);
    }

    sql = fmt::format("insert into snacks(name, size, recipes) values('{}',{},{}) returning id;",name,size,r_id);

    response = db.exec(sql);

    if(response.next()){
        Snack snack(response.get<int>(0),r_id);
        snack.set_name(name);
        snack.set_size(size);

        for(auto ing : ings){
            sql = fmt::format("insert into recipe_to_ingredient(recipe, ingredient, count) values({},{},{});",
                r_id,
                ing.first,
                ing.second
            );
            db.exec(sql);
        }

        db.exec("commit;");
        return snack;
    }

    return Snack(0,0);
}

std::shared_ptr<Snack> SnackGateway::get(int id) {
    auto result = cache.TryGet(id);
    if(result.second){
        return (*result.first).second;
    }else{
        auto db = DbInstance::getInstance();

        std::string sql = fmt::format("select * from snacks where id = {};", id);

        auto response = db.exec(sql);

        if(response.next()){
            Snack snack(response.get<int>(0),response.get<int>(3));
            snack.set_name(response.get<std::string>(1));
            snack.set_size(response.get<int>(2));

            auto ptr = std::make_shared<Snack>(snack);
            cache.Put(id, ptr);

            return ptr;
        }
    }
    throw GatewayException("not found");
}

void SnackGateway::remove(Snack &data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from snacks where id = {};",
            data.get_id());

    db.exec(sql);
}

std::list<Snack> SnackGateway::get_all() {

    auto db = DbInstance::getInstance();

    std::string sql = "select * from snacks;";

    auto response = db.exec(sql);

    std::list<Snack> result;

    while(response.next()){
        Snack snack(response.get<int>(0),response.get<int>(3));
        snack.set_name(response.get<std::string>(1));
        snack.set_size(response.get<int>(2));
        result.push_back(snack);
    }

    return result;
}

std::list<std::pair<int,int>> SnackGateway::get_ingredients(Snack& data){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select ingredient, count from recipe_to_ingredient where recipe = {};",
        data.get_recipe().get_id()
    );

    auto response = db.exec(sql);

    std::list<std::pair<int,int>> result;

    while(response.next()){
        result.push_back(std::make_pair(response.get<int>(0),response.get<int>(1)));
    }

    return result;
}
