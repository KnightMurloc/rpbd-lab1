//
// Created by victor on 16.09.2022.
//

#ifndef LAB1_PROVIDERGATEWAY_H
#define LAB1_PROVIDERGATEWAY_H

#include "../Gateway.h"
#include "Provider.h"

class ProviderGateway : IGateway<Provider> {
public:
    void save(std::shared_ptr<Provider> data) override;

    std::shared_ptr<Provider> create(
        std::string name,
        std::string post_address,
        std::string phone_number,
        std::string fax,
        std::string email,
        int bank_detail
    );

    std::shared_ptr<Provider> get(int id) override;

    void remove(std::shared_ptr<Provider> data) override;

    std::list<std::shared_ptr<Provider>> get_all() override;
};


#endif //LAB1_PROVIDERGATEWAY_H
