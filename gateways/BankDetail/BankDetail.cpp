//
// Created by victor on 15.09.2022.
//

#include "BankDetail.h"

BankDetail::BankDetail(int id) : id(id) {}

int BankDetail::getId() const {
    return id;
}

const std::string &BankDetail::getBankName() const {
    return bank_name;
}

const std::string &BankDetail::getCity() const {
    return city;
}

long long BankDetail::getTin() const {
    return tin;
}

long long BankDetail::getSettlementAccount() const {
    return settlement_account;
}

void BankDetail::setBankName(const std::string &bankName) {
    bank_name = bankName;
}

void BankDetail::setCity(const std::string &city) {
    BankDetail::city = city;
}

void BankDetail::setTin(long long int tin) {
    BankDetail::tin = tin;
}

void BankDetail::setSettlementAccount(long long int settlementAccount) {
    settlement_account = settlementAccount;
}
