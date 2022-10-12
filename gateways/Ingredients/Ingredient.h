//
// Created by victor on 12.09.2022.
//

#ifndef LAB1_INGREDIENT_H
#define LAB1_INGREDIENT_H

#include <memory>
#include <string>
#include "../units.h"
#include "../entity.h"
#include <list>

class Ingredient : public IEntity {
private:
    int id;
    std::string name;
    Unit unit;
public:
    static std::list<std::shared_ptr<Ingredient>> get_great_than_by_id(int id, int count);

    static std::list<std::shared_ptr<Ingredient>> get_less_than_by_id(int id, int count);

    static std::shared_ptr<Ingredient> get(int id);

    static void save(std::shared_ptr<Ingredient> ing);

    static std::shared_ptr<Ingredient> create(std::string name, Unit unit);

    static void remove(std::shared_ptr<Ingredient> ing);

    static std::list<std::shared_ptr<Ingredient>> get_great_then_by_id_filtred_by_name(std::string name, int id, int count);

    static std::list<std::shared_ptr<Ingredient>> get_less_then_by_id_filtred_by_name(std::string name, int id, int count);

    explicit Ingredient(int id);

    [[nodiscard]] int get_id() const;

    [[nodiscard]] const std::string &get_name() const;

    [[nodiscard]] Unit get_unit() const;

    void set_name(const std::string &name);

    void set_unit(Unit unit);
};


#endif //LAB1_INGREDIENT_H
