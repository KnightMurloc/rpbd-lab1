//
// Created by victor on 17.09.2022.
//

#ifndef LAB1_PRODUCTGATEWAY_H
#define LAB1_PRODUCTGATEWAY_H

#include "../Gateway.h"
#include "Product.h"

class ProductGateway : public IGateway<Product> {
public:
    void save(Product &data) override;

    Product create(
        int ingredient_id,
        float price,
        std::string delivery_terms,
        std::string payment_terms,
        int provider_id,
        std::string name
    );

    Product get(int id) override;

    void remove(Product &data) override;

    std::list<Product> get_all() override;
};


#endif //LAB1_PRODUCTGATEWAY_H
