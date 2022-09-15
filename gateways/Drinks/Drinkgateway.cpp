//
// Created by victor on 15.09.2022.
//

#include "Drinkgateway.h"
#include "../DbInstance.h"
#include "Drink.h"
#include "fmt/core.h"
#include <fmt/format.h>

void Drinkgateway::save(Drink &data) {
    auto db = DbInstance::getInstance();

    db.exec("begin transaction;");

    std::string sql = fmt::format("update drinks set name = '{}', strength = {}, size = {}, container = '{}' where id = {}",
        data.getName(),
        data.getStrength(),
        data.getSize(),
        data.getContainer(),
        data.getId()
    );

    db.exec(sql);

    auto rm_ing = data.getRecipe().get_removed_ingridients();
    auto new_ing = data.getRecipe().get_new_ingridients();

    for(auto ing : rm_ing){
        sql = fmt::format("delete from recipe_to_ingredient where ingredient = {};",
            ing
        );
        db.exec(sql);
    }

    for(auto ing : new_ing){
        sql = fmt::format("insert into recipe_to_ingredient(recipe, ingredient, count) values({},{},{});",
            data.getRecipe().get_id(),
            std::get<0>(ing),
            std::get<1>(ing)
        );
        db.exec(sql);
    }

    db.exec("commit;");
}

Drink Drinkgateway::get(int id) {
    return Drink(0, 0);
}

void Drinkgateway::remove(Drink &data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from drinks where id = {};", data.getId());
    db.exec(sql);
}

std::list<Drink> Drinkgateway::get_all() {
    auto db = DbInstance::getInstance();

    std::list<Drink> result;

    auto response = db.exec("select * from drinks;");

    while(response.next()){
        Drink drink(response.get<int>(0),response.get<int>(5));

        drink.setName(response.get<std::string>(1));
        drink.setStrength(response.get<int>(2));
        drink.setSize(response.get<int>(3));
        drink.setContainer(response.get<std::string>(4));

        result.push_back(drink);
    }

    return result;
}

std::list<std::pair<int,int>> Drinkgateway::get_ingredients(Drink& data){
    auto db = DbInstance::getInstance();


    std::string sql = fmt::format("select * from recipe_to_ingredient where recipe = {};",
       data.getRecipe().get_id()
    );

    auto response = db.exec(sql);

    std::list<std::pair<int,int>> result;

    while(response.next()){
        result.push_back(std::make_pair(response.get<int>(1),response.get<int>(2)));
    }
    return result;
}

Drink Drinkgateway::create(std::string name, int strength, int size, std::string container,std::vector<std::pair<int,int>> ings){
    auto db = DbInstance::getInstance();

    db.exec("begin transaction;");
    std::string sql = "insert into recipes(id) values (default) returning id;";
    auto response = db.exec(sql);
    int r_id;
    if(response.next()){
        r_id = response.get<int>(0);
    }else{
        db.exec("rollback;");
        return Drink(0,0);
    }

    sql = fmt::format("insert into drinks(name, strength, size, container, recipes)"
        "values('{}', {}, {}, '{}', {}) returning id;",
        name,
        strength,
        size,
        container,
        r_id
    );

    response = db.exec(sql);
    if(response.next()){
        Drink drink(response.get<int>(0),r_id);
        drink.setName(name);
        drink.setStrength(strength);
        drink.setSize(size);
        drink.setContainer(container);

        for(auto ing : ings){
            sql = fmt::format("insert into recipe_to_ingredient(recipe, ingredient, count) values({},{},{});",
                r_id,
                ing.first,
                ing.second
            );
            db.exec(sql);
        }
        db.exec("commit;");
        return drink;
    }
    db.exec("rollback;");

    return Drink(0,0);
}
