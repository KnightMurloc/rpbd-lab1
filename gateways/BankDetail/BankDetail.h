//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_BANKDETAIL_H
#define LAB1_BANKDETAIL_H

#include <string>

class BankDetail {
private:
    int id;
    std::string bank_name;
    std::string city;
    long long tin;
    long long settlement_account;
public:
    explicit BankDetail(int id);

    [[nodiscard]] int getId() const;

    [[nodiscard]] const std::string &getBankName() const;

    [[nodiscard]] const std::string &getCity() const;

    [[nodiscard]] long long getTin() const;

    [[nodiscard]] long long getSettlementAccount() const;

    void setBankName(const std::string &bankName);

    void setCity(const std::string &city);

    void setTin(long long int tin);

    void setSettlementAccount(long long int settlementAccount);
};


#endif //LAB1_BANKDETAIL_H
