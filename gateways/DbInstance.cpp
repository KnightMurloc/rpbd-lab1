//
// Created by victor on 07.09.2022.
//

//TODO пофиксить диолог создания в сотрудниках
//TODO сделать каскадное удаление впоставщиках и реквизитах

#include "DbInstance.h"
#include <fmt/format.h>

DbInstance::DbInstance() {
    auto const connstr =
            NANODBC_TEXT("DRIVER={PostgreSQL Unicode};SERVER=localhost, 1433;UID=postgres;PWD=miku;database=main;");
    connection = nanodbc::connection(connstr);
    init();
}

DbInstance &DbInstance::getInstance() {
    static DbInstance instance;
    return instance;
}

nanodbc::result DbInstance::exec(std::string sql) {
    return nanodbc::execute(connection, sql);
}

bool DbInstance::check_table_exist(std::string table_name){
    std::string sql = fmt::format("select * from pg_tables where schemaname = 'public' and tablename = '{}';", table_name);


    auto response = exec(sql);
    if(response.next()){
        return true;
    }

    return false;
}

bool DbInstance::check_trigger_exist(std::string name){
    std::string sql = fmt::format("select * from pg_trigger where tgname = '{}';", name);
    auto response = exec(sql);
    if(response.next()){
        return true;
    }

    return false;
}

bool DbInstance::check_function_exist(std::string name){
    std::string sql = fmt::format("select routine_name from information_schema.routines where routine_type = 'FUNCTION' and routine_schema = 'public' and routine_name = '{}';", name);

    auto response = exec(sql);
    if(response.next()){
        return true;
    }

    return false;
}

bool DbInstance::check_enum_exist(std::string name){
    std::string sql = fmt::format("select * from pg_type where typname = '{}';", name);

    auto response = exec(sql);
    if(response.next()){
        return true;
    }

    return false;
}

void DbInstance::init(){
    bool employees = check_table_exist("employees");
    bool bank_detail = check_table_exist("bank_detail");
    bool drinks = check_table_exist("drinks");
    bool ingredients = check_table_exist("ingredients");
    bool orders = check_table_exist("orders");
    bool products = check_table_exist("products");
    bool provider = check_table_exist("provider");
    bool recipe_to_ingredient = check_table_exist("recipe_to_ingredient");
    bool recipes = check_table_exist("recipes");
    bool snacks = check_table_exist("snacks");


//     bool remove_recipe = check_function_exist("remove_recipe");
//     bool remove_recipe_from_snack = check_trigger_exist("remove_recipe_from_snack");
//     bool remove_recipe_from_dink = check_trigger_exist("remove_recipe_from_dink");

//     bool remove_detail = check_function_exist("remove_detail");
    bool remove_detail_from_provider = check_trigger_exist("remove_detail_from_provider");

    bool post = check_enum_exist("post");
    bool units = check_enum_exist("units");

    if(!post){
        std::string sql = "create type post as enum ('manager', 'bartender', 'waiter');";
        exec(sql);
    }
    if(!units){
        std::string sql = "create type units as enum ('ml', 'g');";
        exec(sql);
    }


    if(!orders){
        std::string sql =
        "create table orders("
        "	id serial not null,"
        "	reason text not null,"
        "	order_number int not null,"
        "	post post not null,"
        "	order_date date not null"
        ");";
        exec("begin transaction;");
        exec(sql);

        sql = "create unique index orders_id_uindex on orders (id);";
        exec(sql);
        exec("commit;");
    }

    if(!employees){
        std::string sql =
        "create table employees("
        "    id serial not null,"
        "    first_name varchar not null,"
        "    last_name varchar not null,"
        "    patronymic varchar,"
        "    address varchar not null,"
        "    birth_date date not null,"
        "    salary money not null,"
        "    movement int"
        "        constraint table_name_orders_id_fk"
        "            references orders (id)"
        "                on delete set null,"
        "    post post not null"
        ");";
        exec("begin transaction;");

        exec(sql);

        sql = "create unique index employees_id_uindex on employees (id);";
        exec(sql);
        exec("commit;");
    }

    if(!bank_detail){
        std::string sql =
        "create table bank_detail("
        "    id serial not null,"
        "    bank_name varchar not null,"
        "    city varchar not null,"
        "    tin varchar(10) not null,"
        "    settlement_account varchar(20) not null"
        ");";
        exec("begin transaction;");
        exec(sql);

        sql = "create unique index bank_detail_id_uindex on bank_detail (id);";
        exec(sql);
        exec("commit;");
    }

    if(!provider){
        std::string sql =
        "create table provider("
        "    id serial not null,"
        "    name varchar not null,"
        "    post_address varchar not null,"
        "    phone_number varchar not null,"
        "    fax varchar not null,"
        "    email varchar not null,"
        "    band_detail int"
        "        constraint table_name_bank_detail_id_fk"
        "            references bank_detail (id)"
        "                on delete set null"
        ");";
        exec("begin transaction;");
        exec(sql);

        sql = "create unique index provider_id_uindex on provider (id);";
        exec(sql);
        exec("commit;");
    }

    if(!ingredients){
        std::string sql =
        "create table ingredients("
        "    id serial not null,"
        "    name varchar not null,"
        "    unit units not null"
        ");";
        exec("begin transaction;");
        exec(sql);

        sql = "create unique index ingredients_id_uindex on ingredients (id);";
        exec(sql);
        exec("commit;");
    }

    if(!products){
        std::string sql =
        "create table products("
        "        id int not null,"
        "        ingredient int"
        "            constraint products_ingredients_id_fk"
        "                references ingredients (id)"
        "                    on delete set null,"
        "        price money not null,"
        "        delivery_terms varchar not null,"
        "        payment_terms varchar not null,"
        "        provider int"
        "            constraint products_provider_id_fk"
        "                references provider (id)"
        "                    on delete set null,"
        "        name varchar not null"
        ");";
        exec("begin transaction;");
        exec(sql);

        sql = "create unique index products_id_uindex on products (id);";
        exec(sql);
        exec("commit;");
    }

    if(!recipes){
        std::string sql =
        "create table recipes("
        "    id serial not null"
        ");";

        exec("begin transaction;");
        exec(sql);

        sql = "create unique index recipes_id_uindex on recipes (id);";
        exec(sql);
        exec("commit;");
    }

    if(!recipe_to_ingredient){
        std::string sql =
        "create table recipe_to_ingredient("
        "    recipe int not null"
        "        constraint recipe_to_ingredient_recipes_id_fk"
        "            references recipes (id)"
        "                on delete cascade,"
        "    ingredient int"
        "        constraint recipe_to_ingredient_ingredients_id_fk"
        "            references ingredients (id)"
        "                on delete cascade,"
        "    count int not null"
        ");";
        exec(sql);
    }

    if(!snacks){
        std::string sql =
        "create table snacks("
        "    id serial not null,"
        "    name varchar not null,"
        "    size int not null,"
        "    recipes int not null"
        "        constraint snacks_recipes_id_fk"
        "            references recipes (id)"
        "                on delete cascade"
        ");";
        exec("begin transaction;");
        exec(sql);

        sql = "create unique index snacks_id_uindex on snacks (id);";
        exec(sql);
        exec("commit;");
    }

    if(!drinks){
        std::string sql =
        "create table drinks("
        "    id serial not null,"
        "    name varchar not null,"
        "    strength int not null,"
        "    size int not null,"
        "    container varchar not null,"
        "    recipes int not null"
        "        constraint drinks_recipes_id_fk"
        "            references recipes (id)"
        "                on delete cascade"
        ");";
        exec("begin transaction;");
        exec(sql);

        sql = "create unique index drinks_id_uindex on drinks (id);";
        exec(sql);
        exec("commit;");
    }

//     if(!remove_recipe){
//         std::string sql =
//         "create function remove_recipe() returns trigger"
//         "    language plpgsql"
//         " as "
//         " $$ "
//         " begin "
//         "        delete from recipes where id = old.recipes;"
//         "        return new;"
//         "    end;"
//         " $$; ";
//         exec(sql);
//     }

//     if(!remove_recipe_from_snack){
//         std::string sql =
//         "create trigger remove_recipe_from_snack"
//         "    after delete on snacks"
//         "    for each row"
//         "    execute procedure remove_recipe();";
//         exec(sql);
//     }

//     if(!remove_recipe_from_dink){
//         std::string sql =
//         "create trigger remove_recipe_from_dink"
//         "    after delete on drinks"
//         "    for each row"
//         "    execute procedure remove_recipe();";
//         exec(sql);
//     }

//     if(!remove_detail){
//         std::string sql =
//         "create function remove_detail() returns trigger"
//         "    language plpgsql"
//         " as "
//         " $$ "
//         " begin "
//         "        delete from bank_detail where id = old.bank_detail;"
//         "        return new;"
//         "    end;"
//         "$$;";
//
//         exec(sql);
//     }

    if(!remove_detail_from_provider){
        std::string sql =
        "create trigger remove_detail_from_provider"
        "    after delete on provider"
        "    for each row"
        "    execute procedure remove_detail();";

        exec(sql);
    }

}
