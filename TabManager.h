//
// Created by victor on 09.09.2022.
//

#ifndef LAB1_TABMANAGER_H
#define LAB1_TABMANAGER_H
class TabManager;
#include "Tab.h"

//TODO использовать enum
enum class TabNames{
    EMPLOYEES,
    PRODUCT,
    ORDER,
    SNACK
};

class TabManager {
private:
    Gtk::Notebook* tabs_container;
    std::array<std::unique_ptr<Tab>,8> tabs;
public:

    explicit TabManager(Gtk::Notebook *tabsContainer);

    void select_on_tab(int tab_id, int entry_id);

    int select_dialog(int tab_id);
};


#endif //LAB1_TABMANAGER_H
