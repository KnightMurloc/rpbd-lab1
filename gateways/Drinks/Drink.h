//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_DRINK_H
#define LAB1_DRINK_H

#include <memory>
#include <string>
#include "../Recipe.h"
#include "../entity.h"
#include <vector>

class Drink : public IEntity {
private:
    int id;
    std::string name;
    int strength;
    int size;
    std::string container;
    Recipe recipe;
public:
    static std::list<std::shared_ptr<Drink>> get_great_than_by_id(int id, int count);

    static std::list<std::shared_ptr<Drink>> get_less_than_by_id(int id, int count);

    static std::shared_ptr<Drink> get(int id);

    static std::list<std::pair<int,int>> get_ingredients(std::shared_ptr<Drink> drink);

    static void save(std::shared_ptr<Drink> drink);

    static std::shared_ptr<Drink> create(std::string name, int strength, int size, std::string container,std::vector<std::pair<int,int>> ings);

    static void remove(std::shared_ptr<Drink> drink);

    Drink(int id);

    int get_id() const;

    [[nodiscard]] const std::string &getName() const;

    [[nodiscard]] int getStrength() const;

    [[nodiscard]] int getSize() const;

    [[nodiscard]] const std::string &getContainer() const;

    [[nodiscard]] Recipe &getRecipe();

    void setName(const std::string &name);

    void setStrength(int strength);

    void setSize(int size);

    void setContainer(const std::string &container);
};


#endif //LAB1_DRINK_H
