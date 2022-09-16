cmake_minimum_required(VERSION 3.10)
project(lab1)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)

add_subdirectory(nanodbc)
add_subdirectory(fmt)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS -g)

add_executable(${PROJECT_NAME}
        main.cpp
        LabOne.cpp LabOne.h
        Form.h Form.cpp
        Tab.cpp Tab.h
        EmployeesTab.cpp EmployeesTab.h
        ProductTab.cpp ProductTab.h
        gateways/Gateway.h
        gateways/Employeer/Employeer.cpp gateways/Employeer/Employeer.h
        gateways/posts.h
        gateways/DbInstance.cpp gateways/DbInstance.h
        gateways/Employeer/EmployeerGateway.cpp gateways/Employeer/EmployeerGateway.h
        gateways/Orders/Order.cpp gateways/Orders/Order.h
        gateways/Orders/OrederGateway.cpp gateways/Orders/OrederGateway.h
        OrderTab.cpp OrderTab.h
        TabManager.cpp TabManager.h
        gateways/Snacks/Snacks.cpp gateways/Snacks/Snacks.h
        gateways/units.h
        gateways/Snacks/SnackGateway.cpp gateways/Snacks/SnackGateway.h
        SnackTab.cpp SnackTab.h
        gateways/Ingredients/Ingredient.cpp gateways/Ingredients/Ingredient.h
        gateways/Ingredients/IngredientGateway.cpp gateways/Ingredients/IngredientGateway.h
        IngredientTab.cpp IngredientTab.h
        gateways/Drinks/Drink.cpp gateways/Drinks/Drink.h
        gateways/Recipe.cpp gateways/Recipe.h
        gateways/Drinks/Drinkgateway.cpp gateways/Drinks/Drinkgateway.h
        DrinksTab.cpp DrinksTab.h
        gateways/BankDetail/BankDetail.cpp gateways/BankDetail/BankDetail.h
        gateways/BankDetail/BankDetailgateway.cpp gateways/BankDetail/BankDetailgateway.h
        BankDetailTab.cpp BankDetailTab.h
        gateways/Provider/Provider.cpp gateways/Provider/Provider.h gateways/Provider/ProviderGateway.cpp
        gateways/Provider/ProviderGateway.h ProviderTab.cpp ProviderTab.h)
target_link_directories(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARY_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS} nanodbc fmt/include)
target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} nanodbc fmt)