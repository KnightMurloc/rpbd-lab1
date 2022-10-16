//
// Created by victor on 17.09.2022.
//

#ifndef LAB1_PRODUCT_H
#define LAB1_PRODUCT_H

#include <string>
#include "../Provider/ProviderGateway.h"
#include "../Ingredients/IngredientGateway.h"
#include <memory>
#include "../entity.h"

class Product : public IEntity {
private:
    int id;
//     int ingredient_id = -1;
    std::shared_ptr<Ingredient> ingredient;
    float price;
    std::string delivery_terms;
    std::string payment_terms;
//     int provider_id = -1;
    std::shared_ptr<Provider> provider;
    std::string name;
public:
    static std::list<std::shared_ptr<Product>> get_great_than_by_id(int id, int count);

    static std::list<std::shared_ptr<Product>> get_less_than_by_id(int id, int count);

    static void save(std::shared_ptr<Product> product);

    static std::shared_ptr<Product> create(
        int ingredient_id,
        float price,
        std::string delivery_terms,
        std::string payment_terms,
//         int provider_id,
        std::shared_ptr<Provider> provider,
        std::string name
    );

    static void remove(std::shared_ptr<Product> product);

    explicit Product(int id);

    [[nodiscard]] int get_id() const;

//     [[nodiscard]] int get_ingredient_id() const;

    [[nodiscard]] float get_price() const;

    [[nodiscard]] const std::string &get_delivery_terms() const;

    [[nodiscard]] const std::string &get_payment_terms() const;

//     [[nodiscard]] int get_provider_id() const;

    [[nodiscard]] std::string get_name();

//     void set_ingredient_id(int ingredientId);

    void set_price(float price);

    void set_delivery_terms(const std::string &deliveryTerms);

    void set_payment_terms(const std::string &paymentTerms);

//     void set_provider_id(int providerId);

    void set_ingredient(std::shared_ptr<Ingredient> ingredient);

    void set_provider(std::shared_ptr<Provider> provider);

    std::shared_ptr<Ingredient> get_ingredient();

    std::shared_ptr<Provider> get_provider();

    void set_name(std::string name);
};


#endif //LAB1_PRODUCT_H
