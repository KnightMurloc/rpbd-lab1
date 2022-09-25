//
// Created by victor on 15.09.2022.
//

#include "BankDetailgateway.h"
#include "../DbInstance.h"
#include "BankDetail.h"
#include <iostream>
#include <fmt/format.h>
#include <memory>

template<>
lru_cache_t<int,std::shared_ptr<BankDetail>> IGateway<BankDetail>::cache(CACHE_SIZE);

void BankDetailgateway::save(std::shared_ptr<BankDetail> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("update bank_detail set"
        " bank_name = '{}',"
        " city = '{}',"
        " tin = {},"
        " settlement_account = {}"
        " where id = {};",
        data->getBankName(),
        data->getCity(),
        data->getTin(),
        data->getSettlementAccount(),
        data->get_id()
    );

    db.exec(sql);
}

std::shared_ptr<BankDetail> BankDetailgateway::create(
    std::string bank_name,
    std::string city,
    std::string tin,
    std::string settlement_account){

    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("insert into bank_detail("
        " bank_name,"
        " city,"
        " tin,"
        " settlement_account)"
        "values('{}','{}','{}','{}') returning id;",
        bank_name,
        city,
        tin,
        settlement_account
    );

    auto response = db.exec(sql);

    if(response.next()){
        BankDetail detail(response.get<int>(0));
        detail.setBankName(bank_name);
        detail.setCity(city);
        detail.setTin(tin);
        detail.setSettlementAccount(settlement_account);

        auto ptr = std::make_shared<BankDetail>(detail);
        cache.Put(detail.get_id(), ptr);

        return ptr;
    }

    throw GatewayException("create error");
}

std::shared_ptr<BankDetail> BankDetailgateway::get(int id) {

    auto result = cache.TryGet(id);
    if(result.second){
        return (*result.first).second;
    }else{
        auto db = DbInstance::getInstance();

        std::string sql = fmt::format("select * from bank_detail where id = {};", id);

        auto response = db.exec(sql);

        if(response.next()){
            BankDetail detail(response.get<int>(0));
            detail.setBankName(response.get<std::string>(1));
            detail.setCity(response.get<std::string>(2));
            detail.setTin(response.get<std::string>(3));
            detail.setSettlementAccount(response.get<std::string>(4));

            auto ptr = std::make_shared<BankDetail>(detail);
            cache.Put(id, ptr);

            return ptr;
        }
    }

    throw GatewayException("not found");
}

void BankDetailgateway::remove(std::shared_ptr<BankDetail> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from bank_detail where id = {};", data->get_id());

    db.exec(sql);
}

std::list<std::shared_ptr<BankDetail>> BankDetailgateway::get_all() {
    auto db = DbInstance::getInstance();

    std::list<std::shared_ptr<BankDetail>> result;

    auto response = db.exec("select * from bank_detail;");

    while(response.next()){
        BankDetail detail(response.get<int>(0));
        detail.setBankName(response.get<std::string>(1));
        detail.setCity(response.get<std::string>(2));
        detail.setTin(response.get<std::string>(3));
        detail.setSettlementAccount(response.get<std::string>(4));

        result.push_back(std::make_shared<BankDetail>(detail));
    }

    return result;
}
