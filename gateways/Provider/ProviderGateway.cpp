//
// Created by victor on 16.09.2022.
//

#include "ProviderGateway.h"
#include "../DbInstance.h"
#include "Provider.h"
#include "fmt/format.h"

template<>
lru_cache_t<int,std::shared_ptr<Provider>> IGateway<Provider>::cache(CACHE_SIZE);

void ProviderGateway::save(Provider &data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format(
        "update provider set "
        " name = '{}',"
        " post_address = '{}',"
        " phone_number = '{}',"
        " fax = '{}',"
        " email = '{}',"
        " bank_detail = {}"
        " where id = {};",
        data.get_name(),
        data.get_post_address(),
        data.get_phone_number(),
        data.get_fax(),
        data.get_email(),
        data.get_bank_detail_id() == -1 ? "NULL" : std::to_string(data.get_bank_detail_id()),
        data.get_id()
    );

    db.exec(sql);
}

 Provider ProviderGateway::create(
        std::string name,
        std::string post_address,
        std::string phone_number,
        std::string fax,
        std::string email,
        int bank_detail){
    auto db = DbInstance::getInstance();

    fmt::print("create {}\n",bank_detail);
    std::string sql = fmt::format(
        "insert into provider(name, post_address, phone_number, fax, email, bank_detail) "
        "values('{}', '{}','{}','{}','{}',{}) returning id;",
        name,
        post_address,
        phone_number,
        fax,
        email,
        bank_detail == -1 ? "NULL" : std::to_string(bank_detail)
    );

    auto response = db.exec(sql);

    if(response.next()){
        Provider provider(response.get<int>(0));
        provider.set_name(name);
        provider.set_post_address(post_address);
        provider.set_phone_number(phone_number);
        provider.set_fax(fax);
        provider.set_email(email);
        provider.set_bank_detail_id(bank_detail);

        return provider;
    }

    throw GatewayException("insert error");
}

std::shared_ptr<Provider> ProviderGateway::get(int id) {

    auto result = cache.TryGet(id);
    if(result.second){
        return (*result.first).second;
    }else{
        auto db = DbInstance::getInstance();

        std::string sql = fmt::format("select * from provider where id = {};",id);

        auto response = db.exec(sql);

        if(response.next()){
            Provider provider(response.get<int>(0));
            provider.set_name(response.get<std::string>(1));
            provider.set_post_address(response.get<std::string>(2));
            provider.set_phone_number(response.get<std::string>(3));
            provider.set_fax(response.get<std::string>(4));
            provider.set_email(response.get<std::string>(5));
            if(response.is_null(6)){
                provider.set_bank_detail_id(-1);
            }else{
                provider.set_bank_detail_id(response.get<int>(6));
            }

            auto ptr = std::make_shared<Provider>(provider);
            cache.Put(id, ptr);

            return ptr;
        }
    }
    throw GatewayException("not found");
}

void ProviderGateway::remove(Provider &data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from provider where id = {};", data.get_id());

    db.exec(sql);
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
        if(response.is_null(6)){
            provider.set_bank_detail_id(-1);
        }else{
            provider.set_bank_detail_id(response.get<int>(6));
        }

        result.push_back(provider);
    }

    return result;
}
