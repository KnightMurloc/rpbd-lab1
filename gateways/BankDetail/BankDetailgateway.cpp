//
// Created by victor on 15.09.2022.
//

#include "BankDetailgateway.h"
#include "../DbInstance.h"
#include "BankDetail.h"
#include <iostream>
#include <fmt/format.h>
#include <memory>

// template<>
// lru_cache_t<int,std::shared_ptr<BankDetail>> IGateway<BankDetail>::cache(CACHE_SIZE);

// template<>
// cache<BankDetail> IGateway<BankDetail>::cache_new;

void BankDetailgateway::save(std::shared_ptr<BankDetail> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("update bank_detail set"
        " bank_name = '{}',"
        " city = '{}',"
        " tin = {},"
        " settlement_account = {},"
        " provider = {}"
        " where id = {};",
        escape_string(data->getBankName()),
        escape_string(data->getCity()),
        data->getTin(),
        data->getSettlementAccount(),
        data->get_provider()->get_id(),
        data->get_id()
    );

    db.exec(sql);
}

std::shared_ptr<BankDetail> BankDetailgateway::create(
    std::string bank_name,
    std::string city,
    std::string tin,
    std::string settlement_account,
    std::shared_ptr<Provider> provider

){

    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("insert into bank_detail("
        " bank_name,"
        " city,"
        " tin,"
        " settlement_account,"
        " provider"
        ")"
        "values('{}','{}','{}','{}', {}) returning id;",
        escape_string(bank_name),
        escape_string(city),
        tin,
        settlement_account,
        provider->get_id()
    );

    auto response = db.exec(sql);

    if(response.next()){
        BankDetail detail(response.get<int>(0));
        detail.setBankName(bank_name);
        detail.setCity(city);
        detail.setTin(tin);
        detail.setSettlementAccount(settlement_account);
        detail.set_provider(provider);

        auto ptr = std::make_shared<BankDetail>(detail);
//         cache.Put(detail.get_id(), ptr);
        cache_new.put(ptr);

        return ptr;
    }

    throw GatewayException("create error");
}

std::shared_ptr<BankDetail> BankDetailgateway::get(int id) {

//     auto result = cache.TryGet(id);
//     if(result.second){
//         return (*result.first).second;
//     }else{
//         auto db = DbInstance::getInstance();
//
//         std::string sql = fmt::format("select * from bank_detail where id = {};", id);
//
//         auto response = db.exec(sql);
//
//         if(response.next()){
//             BankDetail detail(response.get<int>(0));
//             detail.setBankName(response.get<std::string>(1));
//             detail.setCity(response.get<std::string>(2));
//             detail.setTin(response.get<std::string>(3));
//             detail.setSettlementAccount(response.get<std::string>(4));
//
//             auto ptr = std::make_shared<BankDetail>(detail);
//             cache.Put(id, ptr);
//
//             return ptr;
//         }
//     }

    if(cache_new.exist(id)){
        return cache_new.get(id);
    }else{
        auto db = DbInstance::getInstance();

        std::string sql = fmt::format("select id, bank_name, city, tin, settlement_account, provider from bank_detail where id = {};", id);

        auto response = db.exec(sql);

        ProviderGateway gateway;

        if(response.next()){

            BankDetail detail(response.get<int>(0));
            detail.setBankName(response.get<std::string>(1));
            detail.setCity(response.get<std::string>(2));
            detail.setTin(response.get<std::string>(3));
            detail.setSettlementAccount(response.get<std::string>(4));
            detail.set_provider(gateway.get(response.get<int>(5)));

            auto ptr = std::make_shared<BankDetail>(detail);
            cache_new.put(ptr);

            return ptr;
        }
    }

    throw GatewayException("not found");
}

void BankDetailgateway::remove(std::shared_ptr<BankDetail> data) {
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("delete from bank_detail where id = {};", data->get_id());

//     cache.Remove(data->get_id());
    cache_new.remove(data->get_id());

    db.exec(sql);
}

std::list<std::shared_ptr<BankDetail>> BankDetailgateway::get_all() {
    auto db = DbInstance::getInstance();

    std::list<std::shared_ptr<BankDetail>> result;

    auto response = db.exec("select id from bank_detail;");

    while(response.next()){
//         BankDetail detail(response.get<int>(0));
//         detail.setBankName(response.get<std::string>(1));
//         detail.setCity(response.get<std::string>(2));
//         detail.setTin(response.get<std::string>(3));
//         detail.setSettlementAccount(response.get<std::string>(4));

        result.push_back(get(response.get<int>(0)));

//         result.push_back(std::make_shared<BankDetail>(detail));
    }

    return result;
}

std::list<std::shared_ptr<BankDetail>> BankDetailgateway::get_great_then_by_id(int min, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from bank_detail where id > {} order by id limit {}",min,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<BankDetail>> result;

    while(response.next()){
//         auto detail = std::make_shared<BankDetail>(response.get<int>(0));
//         detail->setBankName(response.get<std::string>(1));
//         detail->setCity(response.get<std::string>(2));
//         detail->setTin(response.get<std::string>(3));
//         detail->setSettlementAccount(response.get<std::string>(4));
// //         cache.Put(detail->get_id(), detail);
//         cache_new.put(detail);
//         result.push_back(detail);

        result.push_back(get(response.get<int>(0)));
    }
    return result;
}

std::list<std::shared_ptr<BankDetail>> BankDetailgateway::get_less_then_by_id(int min, int count){
    auto db = DbInstance::getInstance();

    std::string sql = fmt::format("select id from bank_detail where id < {} order by id DESC limit {};",min,count);

    auto response = db.exec(sql);

    std::list<std::shared_ptr<BankDetail>> result;

    while(response.next()){
//         auto detail = std::make_shared<BankDetail>(response.get<int>(0));
//         detail->setBankName(response.get<std::string>(1));
//         detail->setCity(response.get<std::string>(2));
//         detail->setTin(response.get<std::string>(3));
//         detail->setSettlementAccount(response.get<std::string>(4));
// //         cache.Put(detail->get_id(), detail);
//         cache_new.put(detail);
//         result.push_back(detail);

        result.push_back(get(response.get<int>(0)));
    }
    return result;
}
