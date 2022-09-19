//
// Created by victor on 07.09.2022.
//

#ifndef LAB1_DBINSTANCE_H
#define LAB1_DBINSTANCE_H

#include <nanodbc/nanodbc.h>
#include <memory>

class DbInstance {
private:
    nanodbc::connection connection;
    DbInstance();

    bool check_table_exist(std::string table_name);
    bool check_function_exist(std::string name);
    bool check_trigger_exist(std::string name);
    bool check_enum_exist(std::string name);

    void init();
public:
    static DbInstance& getInstance();

    nanodbc::result exec(std::string sql);
};


#endif //LAB1_DBINSTANCE_H
