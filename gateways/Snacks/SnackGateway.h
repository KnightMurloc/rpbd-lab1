//
// Created by victor on 11.09.2022.
//

#ifndef LAB1_SNACKGATEWAY_H
#define LAB1_SNACKGATEWAY_H

#include "../Gateway.h"
#include "Snacks.h"
#include <algorithm>
#include <utility>

class SnackGateway : public IGateway<Snack> {
public:
    void save(std::shared_ptr<Snack> data) override;

    std::shared_ptr<Snack> create(std::string name, int size, std::vector<std::pair<int,int>> ings);

    std::shared_ptr<Snack> get(int id) override;

    void remove(std::shared_ptr<Snack> data) override;

    std::list<std::shared_ptr<Snack>> get_all() override;

    std::list<std::pair<int,int>> get_ingredients(std::shared_ptr<Snack> data);

    std::list<std::shared_ptr<Snack>> get_great_then_by_id(int min, int count) override;
    std::list<std::shared_ptr<Snack>> get_less_then_by_id(int max, int count) override;
};

#endif //LAB1_SNACKGATEWAY_H
