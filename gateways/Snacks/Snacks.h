//
// Created by victor on 11.09.2022.
//

#ifndef LAB1_SNACKS_H
#define LAB1_SNACKS_H

#include <string>
#include <vector>
#include <list>
#include "../Recipe.h"
#include "../entity.h"


class Snack : public IEntity {
private:
    int id;
    std::string name;
    int size;

    Recipe recipe;
public:

    explicit Snack(int id);

    [[nodiscard]] Recipe& get_recipe();

    [[nodiscard]] int get_id() const;

    [[nodiscard]] const std::string &get_name() const;

    [[nodiscard]] int get_size() const;

    void set_name(const std::string &name);

    void set_size(int size);
};

#endif //LAB1_SNACKS_H
