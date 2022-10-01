project(gateways)

add_subdirectory(nanodbc)
add_subdirectory(caches)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} STATIC
        Gateway.h
        Employeer/Employeer.cpp Employeer/Employeer.h
        posts.h
        DbInstance.cpp DbInstance.h
        Employeer/EmployeerGateway.cpp Employeer/EmployeerGateway.h
        Orders/Order.cpp Orders/Order.h
        Orders/OrderGateway.cpp Orders/OrderGateway.h
        Snacks/Snacks.cpp Snacks/Snacks.h
        units.h
        Snacks/SnackGateway.cpp Snacks/SnackGateway.h
        Ingredients/Ingredient.cpp Ingredients/Ingredient.h
        Ingredients/IngredientGateway.cpp Ingredients/IngredientGateway.h
        Drinks/Drink.cpp Drinks/Drink.h
        Recipe.cpp Recipe.h
        Drinks/Drinkgateway.cpp Drinks/Drinkgateway.h
        BankDetail/BankDetail.cpp BankDetail/BankDetail.h
        BankDetail/BankDetailgateway.cpp BankDetail/BankDetailgateway.h
        Provider/Provider.cpp Provider/Provider.h
        Provider/ProviderGateway.cpp
        Provider/ProviderGateway.h
        Product/Product.cpp Product/Product.h
        Product/ProductGateway.cpp Product/ProductGateway.h
        SnackOrders/SnackOrder.cpp SnackOrders/SnackOrder.h SnackOrders/SnackOrdergateway.cpp SnackOrders/SnackOrdergateway.h)
target_include_directories(${PROJECT_NAME} PRIVATE caches/include)
target_link_libraries(${PROJECT_NAME} nanodbc caches fmt)
