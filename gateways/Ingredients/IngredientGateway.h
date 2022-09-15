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
    void save(Ingredient &data) override;

    Ingredient create(std::string name, Unit unit);

    Ingredient get(int id) override;

    void remove(Ingredient &data) override;

    std::list<Ingredient> get_all() override;
};


#endif //LAB1_INGREDIENTGATEWAY_H
