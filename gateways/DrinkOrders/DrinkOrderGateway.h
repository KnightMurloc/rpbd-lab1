//
// Created by victor on 02.10.2022.
//

#ifndef LAB1_DRINKORDERGATEWAY_H
#define LAB1_DRINKORDERGATEWAY_H

#include "../Gateway.h"
#include "DrinkOrder.h"

class DrinkOrderGateway : public IGateway<DrinkOrder> {
public:

    std::shared_ptr<DrinkOrder> create(
            int drink_id,
            int waiter_id,
            int table
    );

    void save(std::shared_ptr<DrinkOrder> data) override;

    std::shared_ptr<DrinkOrder> get(int id) override;

    void remove(std::shared_ptr<DrinkOrder> data) override;

    std::list<std::shared_ptr<DrinkOrder>> get_all() override;

    std::list<std::shared_ptr<DrinkOrder>> get_great_then_by_id(int min, int count) override;

    std::list<std::shared_ptr<DrinkOrder>> get_less_then_by_id(int max, int count) override;
};


#endif //LAB1_DRINKORDERGATEWAY_H
