//
// Created by victor on 12.09.2022.
//

#include "IngredientGateway.h"
#include "../DbInstance.h"
#include "Ingredient.h"
#include "fmt/format.h"

void IngredientGateway::save(Ingredient &data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("update ingredients set name = '{}', unit = '{}' where id = {}",
        data.get_name(),
        unit_to_string(data.get_unit()),
        data.get_id()
    );

    db.exec(sql);
}

Ingredient IngredientGateway::get(int id) {

    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select * from ingredients where id = {};", id);

    auto response = db.exec(sql);

    if(response.next()){
        Ingredient ingredient(response.get<int>(0));
        ingredient.set_name(response.get<std::string>(1));
        ingredient.set_unit(string_to_unit(response.get<std::string>(2)));
        return ingredient;
    }
    throw GatewayException("not found");
}

void IngredientGateway::remove(Ingredient &data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from ingredients where id = {};", data.get_id());

    db.exec(sql);
}

std::list<Ingredient> IngredientGateway::get_all() {

    auto db = DbInstance::getInstance();

    std::list<Ingredient> result;

    auto response = db.exec("select * from ingredients;");

    while(response.next()){
        Ingredient ingredient(response.get<int>(0));
        ingredient.set_name(response.get<std::string>(1));
        ingredient.set_unit(string_to_unit(response.get<std::string>(2)));
        result.push_back(ingredient);
    }

    return result;
}

Ingredient IngredientGateway::create(std::string name, Unit unit){
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

        return ing;
    }

    return Ingredient(0);
}
