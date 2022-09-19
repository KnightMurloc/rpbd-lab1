//
// Created by victor on 17.09.2022.
//

#ifndef LAB1_PRODUCT_H
#define LAB1_PRODUCT_H

#include <string>
#include "../Provider/ProviderGateway.h"
#include "../Ingredients/IngredientGateway.h"
#include <memory>

class Product {
private:
    int id;
    int ingredient_id = -1;
    float price;
    std::string delivery_terms;
    std::string payment_terms;
    int provider_id = -1;
    std::string name;
public:
    explicit Product(int id);

    [[nodiscard]] int get_id() const;

    [[nodiscard]] int get_ingredient_id() const;

    [[nodiscard]] float get_price() const;

    [[nodiscard]] const std::string &get_delivery_terms() const;

    [[nodiscard]] const std::string &get_payment_terms() const;

    [[nodiscard]] int get_provider_id() const;

    [[nodiscard]] std::string get_name();

    void set_ingredient_id(int ingredientId);

    void set_price(float price);

    void set_delivery_terms(const std::string &deliveryTerms);

    void set_payment_terms(const std::string &paymentTerms);

    void set_provider_id(int providerId);

    std::shared_ptr<Ingredient> get_ingredient();

    std::shared_ptr<Provider> get_provider();

    void set_name(std::string name);
};


#endif //LAB1_PRODUCT_H
