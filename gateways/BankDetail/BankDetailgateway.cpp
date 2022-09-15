//
// Created by victor on 15.09.2022.
//

#include "BankDetailgateway.h"
#include "../DbInstance.h"
#include <iostream>

void BankDetailgateway::save(BankDetail &data) {

}

BankDetail BankDetailgateway::get(int id) {
    return BankDetail(0);
}

void BankDetailgateway::remove(BankDetail &data) {

}

std::list<BankDetail> BankDetailgateway::get_all() {
    auto db = DbInstance::getInstance();

    std::list<BankDetail> result;

    auto response = db.exec("select * from bank_detail;");

    while(response.next()){
        BankDetail detail(response.get<int>(0));
        detail.setBankName(response.get<std::string>(1));
        detail.setCity(response.get<std::string>(2));
        detail.setTin(response.get<long long>(3));
        detail.setSettlementAccount(response.get<long long>(4));

        result.push_back(detail);
    }

    return result;
}
