//
// Created by victor on 12.09.2022.
//

#ifndef LAB1_INGREDIENT_H
#define LAB1_INGREDIENT_H

#include <string>
#include "../units.h"
#include "../entity.h"

class Ingredient : public IEntity {
private:
    int id;
    std::string name;
    Unit unit;
public:
    explicit Ingredient(int id);

    [[nodiscard]] int get_id() const;

    [[nodiscard]] const std::string &get_name() const;

    [[nodiscard]] Unit get_unit() const;

    void set_name(const std::string &name);

    void set_unit(Unit unit);
};


#endif //LAB1_INGREDIENT_H
