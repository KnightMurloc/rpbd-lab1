//
// Created by victor on 15.09.2022.
//

#ifndef LAB1_BANKDETAIL_H
#define LAB1_BANKDETAIL_H

#include <string>
#include "../entity.h"
#include <memory>
#include <list>
#include "../Provider/ProviderGateway.h"

class BankDetail : public IEntity {
private:
    int id;
    std::string bank_name;
    std::string city;
    std::string tin;
    std::string settlement_account;
    std::shared_ptr<Provider> provider;
public:

    static std::shared_ptr<BankDetail> get(int id);

    static std::list<std::shared_ptr<BankDetail>> get_great_than_by_id(int id, int count);

    static std::list<std::shared_ptr<BankDetail>> get_less_than_by_id(int id, int count);

    static void save(std::shared_ptr<BankDetail> detail);

    static std::shared_ptr<BankDetail> create(
        std::string bank_name,
        std::string city,
        std::string tin,
        std::string settlement_account,
        std::shared_ptr<Provider> provider
    );

    static void remove(std::shared_ptr<BankDetail> detail);

    explicit BankDetail(int id);

    [[nodiscard]] int get_id() const;

    [[nodiscard]] const std::string &getBankName() const;

    [[nodiscard]] const std::string &getCity() const;

    [[nodiscard]] std::string getTin() const;

    [[nodiscard]] std::string getSettlementAccount() const;

    [[nodiscard]] std::shared_ptr<Provider> get_provider();

    void setBankName(const std::string &bankName);

    void setCity(const std::string &city);

    void setTin(std::string tin);

    void setSettlementAccount(std::string settlementAccount);

    void set_provider(std::shared_ptr<Provider> provider);
};


#endif //LAB1_BANKDETAIL_H
