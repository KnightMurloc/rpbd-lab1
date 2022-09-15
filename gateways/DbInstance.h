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
public:
    static DbInstance& getInstance();

    nanodbc::result exec(std::string sql);
};


#endif //LAB1_DBINSTANCE_H
