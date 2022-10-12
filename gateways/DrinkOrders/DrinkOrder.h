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
    std::shared_ptr<Drink> drink;
    std::shared_ptr<Employeer> waiter;
//     int drink;
//     int waiter;
    int table;
public:
    static std::list<std::shared_ptr<DrinkOrder>> get_great_than_by_id(int id, int count);

    static std::list<std::shared_ptr<DrinkOrder>> get_less_than_by_id(int id, int count);

    static void save(std::shared_ptr<DrinkOrder> order);

    static std::shared_ptr<DrinkOrder> create(
            std::shared_ptr<Drink> drink,
            std::shared_ptr<Employeer> waiter,
            int table
    );

    static void remove(std::shared_ptr<DrinkOrder> order);

    explicit DrinkOrder(int id);

    [[nodiscard]] int get_id() const override;

    [[nodiscard]] int get_table() const;

    [[nodiscard]] std::shared_ptr<Drink> get_drink();

    [[nodiscard]] std::shared_ptr<Employeer> get_waiter();

    void set_drink(std::shared_ptr<Drink>);

    void set_waiter(std::shared_ptr<Employeer>);

    void set_table(int table);
};


#endif //LAB1_DRINKORDER_H
