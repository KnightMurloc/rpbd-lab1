//
// Created by victor on 15.09.2022.
//

#include "Drinkgateway.h"
#include "../DbInstance.h"

void Drinkgateway::save(Drink &data) {

}

Drink Drinkgateway::get(int id) {
    return Drink(0, 0);
}

void Drinkgateway::remove(Drink &data) {

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
