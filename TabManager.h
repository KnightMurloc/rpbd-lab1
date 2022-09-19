//
// Created by victor on 09.09.2022.
//

#ifndef LAB1_TABMANAGER_H
#define LAB1_TABMANAGER_H
class TabManager;
#include "Tab.h"

//TODO использовать enum
enum class TabName{
    EMPLOYEES,
    PRODUCT,
    ORDER,
    SNACK,
    INGREDIENTS,
    DRINKS,
    BACK_DETAIL,
    PROVIDER
};

class TabManager {
private:
    Gtk::Notebook* tabs_container;
    std::array<std::unique_ptr<Tab>,8> tabs;
public:

    explicit TabManager(Gtk::Notebook *tabsContainer);

    void select_on_tab(TabName tab_name, int entry_id);

    int select_dialog(TabName tab_name);

    void remove_on_tab(TabName tab_name, int id);
};


#endif //LAB1_TABMANAGER_H
