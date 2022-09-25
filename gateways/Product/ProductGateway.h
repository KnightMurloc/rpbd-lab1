//
// Created by victor on 17.09.2022.
//

#ifndef LAB1_PRODUCTGATEWAY_H
#define LAB1_PRODUCTGATEWAY_H

#include "../Gateway.h"
#include "Product.h"

class ProductGateway : public IGateway<Product> {
public:
    void save(std::shared_ptr<Product> data) override;

    std::shared_ptr<Product> create(
        int ingredient_id,
        float price,
        std::string delivery_terms,
        std::string payment_terms,
        int provider_id,
        std::string name
    );

    std::shared_ptr<Product> get(int id) override;

    void remove(std::shared_ptr<Product> data) override;

    std::list<std::shared_ptr<Product>> get_all() override;
};


#endif //LAB1_PRODUCTGATEWAY_H
