//
// Created by victor on 02.10.2022.
//

#ifndef LAB1_DRINKORDER_H
#define LAB1_DRINKORDER_H

#include "../entity.h"
#include "../Drinks/Drink.h"
#include "../Employeer/Employeer.h"
#include <memory>

class DrinkOrder : public IEntity {
private:
    int id;
    int drink;
    int waiter;
    int table;
public:
    explicit DrinkOrder(int id);

    [[nodiscard]] int get_id() const override;

    [[nodiscard]] int get_drink_id() const;

    [[nodiscard]] int get_waiter_id() const;

    [[nodiscard]] int get_table() const;

    [[nodiscard]] std::shared_ptr<Drink> get_drink();

    [[nodiscard]] std::shared_ptr<Employeer> get_waiter();

    void set_drink_id(int drink);

    void set_waiter_id(int waiter);

    void set_table(int table);
};


#endif //LAB1_DRINKORDER_H
