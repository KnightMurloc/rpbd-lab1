//
// Created by victor on 12.09.2022.
//

#ifndef LAB1_INGREDIENTGATEWAY_H
#define LAB1_INGREDIENTGATEWAY_H

#include "../Gateway.h"
#include "Ingredient.h"
#include <utility>
#include <vector>

class IngredientGateway : public IGateway<Ingredient> {
public:
    void save(std::shared_ptr<Ingredient> data) override;

    std::shared_ptr<Ingredient> create(std::string name, Unit unit);

    std::shared_ptr<Ingredient> get(int id) override;

    void remove(std::shared_ptr<Ingredient> data) override;

    std::list<std::shared_ptr<Ingredient>> get_all() override;

    int get_min();
    int get_max();

    std::list<std::shared_ptr<Ingredient>> get_great_then_by_name(std::string name, int id, int count);

    std::list<std::shared_ptr<Ingredient>> get_less_then_by_name(std::string name, int id, int count);
//     std::list<std::shared_ptr<Ingredient>> get_range_by_id(int min, int max, int count);
    std::list<std::shared_ptr<Ingredient>> get_great_then_by_id(int min, int count) override;
    std::list<std::shared_ptr<Ingredient>> get_less_then_by_id(int max, int count) override;
};


#endif //LAB1_INGREDIENTGATEWAY_H
