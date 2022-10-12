//
// Created by victor on 15.09.2022.
//

#include "BankDetail.h"
#include "BankDetailgateway.h"
#include <iostream>

BankDetail::BankDetail(int id) : id(id) {}

int BankDetail::get_id() const {
    return id;
}

const std::string &BankDetail::getBankName() const {
    return bank_name;
}

const std::string &BankDetail::getCity() const {
    return city;
}

std::string BankDetail::getTin() const {
    return tin;
}

std::string BankDetail::getSettlementAccount() const {
    return settlement_account;
}

void BankDetail::setBankName(const std::string &bankName) {
    bank_name = bankName;
}

void BankDetail::setCity(const std::string &city) {
    BankDetail::city = city;
}

void BankDetail::setTin(std::string tin) {
    BankDetail::tin = tin;
}

void BankDetail::setSettlementAccount(std::string settlementAccount) {
    settlement_account = settlementAccount;
}

std::shared_ptr<BankDetail> BankDetail::get(int id){
    BankDetailgateway gateway;
    return gateway.get(id);
}

std::list<std::shared_ptr<BankDetail>> BankDetail::get_great_than_by_id(int id, int count) {
    BankDetailgateway gateway;
    return gateway.get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<BankDetail>> BankDetail::get_less_than_by_id(int id, int count) {
    BankDetailgateway gateway;
    return gateway.get_less_then_by_id(id,count);
}

void BankDetail::save(std::shared_ptr<BankDetail> detail){
    BankDetailgateway gateway;

    gateway.save(detail);
}

std::shared_ptr<BankDetail> BankDetail::create(
        std::string bank_name,
        std::string city,
        std::string tin,
        std::string settlement_account
    ){
    BankDetailgateway gateway;
    return gateway.create(bank_name,city,tin,settlement_account);
}

void BankDetail::remove(std::shared_ptr<BankDetail> detail){
    BankDetailgateway gateway;
    gateway.remove(detail);
}
