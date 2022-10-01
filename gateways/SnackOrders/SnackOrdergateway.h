//
// Created by victor on 30.09.2022.
//

#ifndef LAB1_SNACKORDERGATEWAY_H
#define LAB1_SNACKORDERGATEWAY_H

#include "../Gateway.h"
#include "SnackOrder.h"

class SnackOrdergateway : public IGateway<SnackOrder> {
public:

    std::shared_ptr<SnackOrder> create(
        int snack_id,
        int waiter_id,
        int table
    );

    void save(std::shared_ptr<SnackOrder> data) override;

    std::shared_ptr<SnackOrder> get(int id) override;

    void remove(std::shared_ptr<SnackOrder> data) override;

    std::list<std::shared_ptr<SnackOrder>> get_all() override;

    std::list<std::shared_ptr<SnackOrder>> get_great_then_by_id(int min, int count) override;

    std::list<std::shared_ptr<SnackOrder>> get_less_then_by_id(int max, int count) override;
};


#endif //LAB1_SNACKORDERGATEWAY_H
