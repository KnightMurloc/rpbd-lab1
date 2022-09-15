//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_DRINKGATEWAY_H
#define LAB1_DRINKGATEWAY_H

#include "../Gateway.h"
#include "Drink.h"

class Drinkgateway : public IGateway<Drink> {
public:
    void save(Drink &data) override;

    Drink get(int id) override;

    void remove(Drink &data) override;

    std::list<Drink> get_all() override;
};


#endif //LAB1_DRINKGATEWAY_H
