//
// Created by victor on 16.09.2022.
//

#include "ProviderGateway.h"
#include "../DbInstance.h"

void ProviderGateway::save(Provider &data) {

}

Provider ProviderGateway::get(int id) {
    return Provider(0);
}

void ProviderGateway::remove(Provider &data) {

}

std::list<Provider> ProviderGateway::get_all() {

    auto db = DbInstance::getInstance();

    std::string sql = "select * from provider;";

    auto response = db.exec(sql);

    std::list<Provider> result;

    while(response.next()){
        Provider provider(response.get<int>(0));
        provider.set_name(response.get<std::string>(1));
        provider.set_post_address(response.get<std::string>(2));
        provider.set_phone_number(response.get<std::string>(3));
        provider.set_fax(response.get<std::string>(4));
        provider.set_email(response.get<std::string>(5));
        provider.set_bank_detail(response.get<int>(6));

        result.push_back(provider);
    }

    return result;
}
