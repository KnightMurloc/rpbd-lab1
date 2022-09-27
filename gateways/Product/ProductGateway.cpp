//
// Created by victor on 17.09.2022.
//

#include "ProductGateway.h"
#include "../DbInstance.h"
#include "Product.h"
#include "fmt/format.h"
#include <memory>
#include <string>

template<>
lru_cache_t<int,std::shared_ptr<Product>> IGateway<Product>::cache(CACHE_SIZE);

void ProductGateway::save(std::shared_ptr<Product> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format(
        "update products set "
        " ingredient = {},"
        " price = {},"
        " delivery_terms = '{}',"
        " payment_terms = '{}',"
        " provider = {},"
        " name = '{}'"
        " where id = {};",
        data->get_ingredient_id(),
        data->get_price(),
        data->get_delivery_terms(),
        data->get_payment_terms(),
        data->get_provider_id(),
        data->get_name(),
        data->get_id()
    );

    db.exec(sql);
}

std::shared_ptr<Product> ProductGateway::create(
    int ingredient_id,
    float price,
    std::string delivery_terms,
    std::string payment_terms,
    int provider_id,
    std::string name){

    auto db = DbInstance::getInstance();

    std::string sql = fmt::format(
        "insert into products(ingredient, price, delivery_terms, payment_terms, provider, name)"
        " values({},{},'{}','{}',{},'{}') returning id;",
        ingredient_id == -1 ? "NULL" : std::to_string(ingredient_id),
        price,
        delivery_terms,
        payment_terms,
        provider_id == -1 ? "NULL" : std::to_string(provider_id),
        name
    );

    auto response = db.exec(sql);
    if(response.next()){
        Product product(response.get<int>(0));
        product.set_ingredient_id(ingredient_id);
        product.set_price(price);
        product.set_delivery_terms(delivery_terms);
        product.set_payment_terms(payment_terms);
        product.set_provider_id(provider_id);
        product.set_name(name);

        auto ptr = std::make_shared<Product>(product);

        cache.Put(ptr->get_id(), ptr);

        return ptr;
    }

    throw GatewayException("create erorr");
}

std::shared_ptr<Product> ProductGateway::get(int id) {

    auto result = cache.TryGet(id);
    if(result.second){
        return (*result.first).second;
    }else{
        auto db = DbInstance::getInstance();

        std::string sql = fmt::format("select * from products where id = {};", id);

        auto response = db.exec(sql);
            if(response.next()){
                Product product(response.get<int>(0));
                if(!response.is_null(1)) {
                    product.set_ingredient_id(response.get<int>(1));
                }
                product.set_price(response.get<float>(2));
                product.set_delivery_terms(response.get<std::string>(3));
                product.set_payment_terms(response.get<std::string>(4));
                if(!response.is_null(5)) {
                    product.set_provider_id(response.get<int>(5));
                }
                product.set_name(response.get<std::string>(6));

                auto ptr = std::make_shared<Product>(product);
                cache.Put(id, ptr);

                return ptr;
        }
    }

    throw GatewayException("not found");
}

void ProductGateway::remove(std::shared_ptr<Product> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from products where id = {};",data->get_id());

    cache.Remove(data->get_id());

    db.exec(sql);
}

std::list<std::shared_ptr<Product>> ProductGateway::get_all() {

    auto db = DbInstance::getInstance();

    std::list<std::shared_ptr<Product>> result;

    auto response = db.exec("select * from products;");

    while (response.next()){
        Product product(response.get<int>(0));
        if(!response.is_null(1)) {
            product.set_ingredient_id(response.get<int>(1));
        }
        product.set_price(response.get<float>(2));
        product.set_delivery_terms(response.get<std::string>(3));
        product.set_payment_terms(response.get<std::string>(4));
        if(!response.is_null(5)) {
            product.set_provider_id(response.get<int>(5));
        }
        product.set_name(response.get<std::string>(6));

        auto ptr = std::make_shared<Product>(product);

        cache.Put(ptr->get_id(), ptr);

        result.push_back(ptr);
    }

    return result;
}

std::list<std::shared_ptr<Product>> ProductGateway::get_great_then_by_id(int min, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select * from products where id > {} order by id limit {}",min,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Product>> result;

    while(response.next()){
        auto product = std::make_shared<Product>(response.get<int>(0));
        if(!response.is_null(1)) {
            product->set_ingredient_id(response.get<int>(1));
        }
        product->set_price(response.get<float>(2));
        product->set_delivery_terms(response.get<std::string>(3));
        product->set_payment_terms(response.get<std::string>(4));
        if(!response.is_null(5)) {
            product->set_provider_id(response.get<int>(5));
        }
        product->set_name(response.get<std::string>(6));
        cache.Put(product->get_id(), product);
        result.push_back(product);
    }
    return result;
}

std::list<std::shared_ptr<Product>> ProductGateway::get_less_then_by_id(int min, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select * from products where id < {} order by id DESC limit {};",min,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<Product>> result;

    while(response.next()){
        auto product = std::make_shared<Product>(response.get<int>(0));
        if(!response.is_null(1)) {
            product->set_ingredient_id(response.get<int>(1));
        }
        product->set_price(response.get<float>(2));
        product->set_delivery_terms(response.get<std::string>(3));
        product->set_payment_terms(response.get<std::string>(4));
        if(!response.is_null(5)) {
            product->set_provider_id(response.get<int>(5));
        }
        product->set_name(response.get<std::string>(6));
        cache.Put(product->get_id(), product);
        result.push_back(product);
    }
    return result;
}
