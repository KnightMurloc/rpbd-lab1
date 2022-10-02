cmake_minimum_required(VERSION 3.10)
project(lab1)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)

add_subdirectory(gateways)
#add_subdirectory(nanodbc)
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

        OrderTab.cpp OrderTab.h
        TabManager.cpp TabManager.h

        SnackTab.cpp SnackTab.h

        IngredientTab.cpp IngredientTab.h

        DrinksTab.cpp DrinksTab.h

        BankDetailTab.cpp BankDetailTab.h

        ProviderTab.cpp ProviderTab.h
        EntityList.h EntityList.inc
        SnackOrdersTab.cpp SnackOrdersTab.h
        DrinkOrdersTab.cpp DrinkOrdersTab.h)
target_link_directories(${PROJECT_NAME} PRIVATE ${GTKMM_LIBRARY_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS} fmt/include)
target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} fmt gateways)
