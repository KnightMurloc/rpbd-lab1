//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_DRINKGATEWAY_H
#define LAB1_DRINKGATEWAY_H

#include "../Gateway.h"
#include "Drink.h"
#include <vector>

class Drinkgateway : public IGateway<Drink> {
public:
    void save(std::shared_ptr<Drink> data) override;

    std::shared_ptr<Drink> create(std::string name, int strength, int size, std::string container,std::vector<std::pair<int,int>> ings);

    std::shared_ptr<Drink> get(int id) override;

    void remove(std::shared_ptr<Drink> data) override;

    std::list<std::shared_ptr<Drink>> get_all() override;

    std::list<std::pair<int,int>> get_ingredients(std::shared_ptr<Drink> data);
};


#endif //LAB1_DRINKGATEWAY_H
