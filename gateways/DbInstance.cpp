//
// Created by victor on 07.09.2022.
//

#include "DbInstance.h"

DbInstance::DbInstance() {
    auto const connstr =
            NANODBC_TEXT("DRIVER={PostgreSQL Unicode};SERVER=localhost, 1433;UID=postgres;PWD=miku;database=main;");
    connection = nanodbc::connection(connstr);

}

DbInstance &DbInstance::getInstance() {
    static DbInstance instance;
    return instance;
}

nanodbc::result DbInstance::exec(std::string sql) {
    return nanodbc::execute(connection, sql);
}
