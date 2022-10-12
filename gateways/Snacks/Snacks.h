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
    static std::list<std::shared_ptr<Snack>> get_great_than_by_id(int id, int count);

    static std::list<std::shared_ptr<Snack>> get_less_than_by_id(int id, int count);

    static std::list<std::pair<int,int>> get_ingredients(std::shared_ptr<Snack> data);

    static std::shared_ptr<Snack> get(int id);

    static std::shared_ptr<Snack> create(std::string name, int size, std::vector<std::pair<int,int>> ings);

    static void remove(std::shared_ptr<Snack> snack);

    static void save(std::shared_ptr<Snack> snack);

    explicit Snack(int id);

    [[nodiscard]] Recipe& get_recipe();

    [[nodiscard]] int get_id() const;

    [[nodiscard]] const std::string &get_name() const;

    [[nodiscard]] int get_size() const;

    void set_name(const std::string &name);

    void set_size(int size);
};

#endif //LAB1_SNACKS_H
