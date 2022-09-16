//
// Created by victor on 16.09.2022.
//

#ifndef LAB1_PROVIDERGATEWAY_H
#define LAB1_PROVIDERGATEWAY_H

#include "../Gateway.h"
#include "Provider.h"

class ProviderGateway : IGateway<Provider> {
public:
    void save(Provider &data) override;

    Provider get(int id) override;

    void remove(Provider &data) override;

    std::list<Provider> get_all() override;
};


#endif //LAB1_PROVIDERGATEWAY_H
