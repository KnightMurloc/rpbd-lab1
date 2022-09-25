//
// Created by victor on 12.09.2022.
//

#include "IngredientGateway.h"
#include "../DbInstance.h"
#include "Ingredient.h"
#include "fmt/format.h"

template<>
lru_cache_t<int,std::shared_ptr<Ingredient>> IGateway<Ingredient>::cache(CACHE_SIZE);

void IngredientGateway::save(std::shared_ptr<Ingredient> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("update ingredients set name = '{}', unit = '{}' where id = {}",
        data->get_name(),
        unit_to_string(data->get_unit()),
        data->get_id()
    );

    db.exec(sql);
}

std::shared_ptr<Ingredient> IngredientGateway::get(int id) {

    auto result = cache.TryGet(id);

    if(result.second){
        return (*result.first).second;
    }else{
        auto db = DbInstance::getInstance();

        std::string sql = fmt::format("select * from ingredients where id = {};", id);

        auto response = db.exec(sql);

        if(response.next()){
            Ingredient ingredient(response.get<int>(0));
            ingredient.set_name(response.get<std::string>(1));
            ingredient.set_unit(string_to_unit(response.get<std::string>(2)));

            auto ptr = std::make_shared<Ingredient>(ingredient);
            cache.Put(id, ptr);

            return ptr;
        }
    }
    throw GatewayException("not found");
}

void IngredientGateway::remove(std::shared_ptr<Ingredient> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from ingredients where id = {};", data->get_id());

    db.exec(sql);

    cache.Remove(data->get_id());
}

std::list<std::shared_ptr<Ingredient>> IngredientGateway::get_all() {

    auto db = DbInstance::getInstance();

    std::list<std::shared_ptr<Ingredient>> result;

    auto response = db.exec("select * from ingredients;");

    while(response.next()){
        Ingredient ingredient(response.get<int>(0));
        ingredient.set_name(response.get<std::string>(1));
        ingredient.set_unit(string_to_unit(response.get<std::string>(2)));

        auto ptr = std::make_shared<Ingredient>(ingredient);

        cache.Put(ptr->get_id(),ptr);

        result.push_back(ptr);
    }

    return result;
}

std::shared_ptr<Ingredient> IngredientGateway::create(std::string name, Unit unit){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("insert into ingredients(name, unit) values('{}', '{}') returning id;",
        name,
        unit_to_string(unit)
    );

    auto response = db.exec(sql);

    if(response.next()){
        Ingredient ing(response.get<int>(0));
        ing.set_name(name);
        ing.set_unit(unit);

        auto ptr = std::make_shared<Ingredient>(ing);

        cache.Put(ptr->get_id(),ptr);

        return ptr;
    }

    throw GatewayException("create error");
}

// std::list<std::shared_ptr<Ingredient>> IngredientGateway::get_range_by_id(int min, int max, int count){
//     auto db = DbInstance::getInstance();
//
//     std::string sql = fmt::format("select * from ingredients where id > {} and id < {} order by id limit {};", min, max, count);
//
//     auto response = db.exec(sql);
//
//     std::list<std::shared_ptr<Ingredient>> result;
//
//     while(response.next()){
//         auto ing = std::make_shared<Ingredient>(response.get<int>(0));
//         ing->set_name(response.get<std::string>(1));
//         ing->set_unit(string_to_unit(response.get<std::string>(2)));
//         cache.Put(ing->get_id(), ing);
//         result.push_back(ing);
//     }
//
//     return result;
// }

std::list<std::shared_ptr<Ingredient>> IngredientGateway::get_great_then_by_id(int min, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select * from ingredients where id > {} order by id limit {}",min,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Ingredient>> result;

    while(response.next()){
        auto ing = std::make_shared<Ingredient>(response.get<int>(0));
        ing->set_name(response.get<std::string>(1));
        ing->set_unit(string_to_unit(response.get<std::string>(2)));
        cache.Put(ing->get_id(), ing);
        result.push_back(ing);
    }
    return result;
}

std::list<std::shared_ptr<Ingredient>> IngredientGateway::get_less_then_by_id(int min, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select * from ingredients where id < {} order by id DESC limit {};",min,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Ingredient>> result;

    while(response.next()){
        auto ing = std::make_shared<Ingredient>(response.get<int>(0));
        ing->set_name(response.get<std::string>(1));
        ing->set_unit(string_to_unit(response.get<std::string>(2)));
        cache.Put(ing->get_id(), ing);
        result.push_back(ing);
    }
    return result;
}

int IngredientGateway::get_min(){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select min(id) from ingredients;");

    auto response = db.exec(sql);

    if(response.next()){
        return response.get<int>(0);
    }

    throw GatewayException("get min error");
}

int IngredientGateway::get_max(){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select max(id) from ingredients;");

    auto response = db.exec(sql);

    if(response.next()){
        return response.get<int>(0);
    }

    throw GatewayException("get max error");
}

std::list<std::shared_ptr<Ingredient>> IngredientGateway::get_great_then_by_name(std::string name, int id, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select * from ingredients where lower(name) like '%{}%' and id > {} order by id limit {};", name,id,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Ingredient>> result;

    while(response.next()){
        auto ing = std::make_shared<Ingredient>(response.get<int>(0));
        ing->set_name(response.get<std::string>(1));
        ing->set_unit(string_to_unit(response.get<std::string>(2)));
        cache.Put(ing->get_id(), ing);
        result.push_back(ing);
    }
    return result;
}

std::list<std::shared_ptr<Ingredient>> IngredientGateway::get_less_then_by_name(std::string name, int id, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select * from ingredients where lower(name) like '%{}%' and id < {} order by id DESC limit {};", name,id,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Ingredient>> result;

    while(response.next()){
        auto ing = std::make_shared<Ingredient>(response.get<int>(0));
        ing->set_name(response.get<std::string>(1));
        ing->set_unit(string_to_unit(response.get<std::string>(2)));
        cache.Put(ing->get_id(), ing);
        result.push_back(ing);
    }
    return result;
}
