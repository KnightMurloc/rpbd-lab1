//
// Created by victor on 17.09.2022.
//

#include "Product.h"

Product::Product(int id) : id(id) {}

int Product::get_id() const {
    return id;
}

int Product::get_ingredient_id() const {
    return ingredient_id;
}

float Product::get_price() const {
    return price;
}

const std::string &Product::get_delivery_terms() const {
    return delivery_terms;
}

const std::string &Product::get_payment_terms() const {
    return payment_terms;
}

int Product::get_provider_id() const {
    return provider_id;
}

void Product::set_ingredient_id(int ingredientId) {
    ingredient_id = ingredientId;
}

void Product::set_price(float price) {
    Product::price = price;
}

void Product::set_delivery_terms(const std::string &deliveryTerms) {
    delivery_terms = deliveryTerms;
}

void Product::set_payment_terms(const std::string &paymentTerms) {
    payment_terms = paymentTerms;
}

void Product::set_provider_id(int providerId) {
    provider_id = providerId;
}

std::shared_ptr<Ingredient> Product::get_ingredient() {
    if(ingredient_id == -1){
        throw GatewayException("not found");
    }

    try {
        IngredientGateway gateway;
        return gateway.get(ingredient_id);
    } catch (GatewayException&) {
        ingredient_id = -1;
        throw GatewayException("not found");
    }
}

std::shared_ptr<Provider> Product::get_provider() {
    if(provider_id == -1){
        throw GatewayException("not found");
    }

    try {
        ProviderGateway gateway;
        return gateway.get(provider_id);
    } catch (GatewayException&) {
        provider_id = -1;
        throw GatewayException("not found");
    }
}

std::string Product::get_name(){
    return name;
}

void Product::set_name(std::string name){
    this->name = name;
}
