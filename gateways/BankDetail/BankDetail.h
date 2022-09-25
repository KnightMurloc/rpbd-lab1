//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_BANKDETAIL_H
#define LAB1_BANKDETAIL_H

#include <string>
#include "../entity.h"

class BankDetail : public IEntity {
private:
    int id;
    std::string bank_name;
    std::string city;
    std::string tin;
    std::string settlement_account;
public:
    explicit BankDetail(int id);

    [[nodiscard]] int get_id() const;

    [[nodiscard]] const std::string &getBankName() const;

    [[nodiscard]] const std::string &getCity() const;

    [[nodiscard]] std::string getTin() const;

    [[nodiscard]] std::string getSettlementAccount() const;

    void setBankName(const std::string &bankName);

    void setCity(const std::string &city);

    void setTin(std::string tin);

    void setSettlementAccount(std::string settlementAccount);
};


#endif //LAB1_BANKDETAIL_H
