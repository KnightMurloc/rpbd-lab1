//
// Created by victor on 15.09.2022.
//

#include "BankDetail.h"
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