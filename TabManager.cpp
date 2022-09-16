//
// Created by victor on 09.09.2022.
//

#include "TabManager.h"
#include "EmployeesTab.h"
#include "ProductTab.h"
#include "OrderTab.h"
#include "SnackTab.h"
#include "IngredientTab.h"
#include "DrinksTab.h"
#include "BankDetailTab.h"
#include "ProviderTab.h"

TabManager::TabManager(Gtk::Notebook *tabsContainer) : tabs_container(tabsContainer) {
    tabs[0] = std::make_unique<EmployeesTab>(this);
    tabs[1] = std::make_unique<ProductTab>(this);
    tabs[2] = std::make_unique<OrderTab>(this);
    tabs[3] = std::make_unique<SnackTab>(this);
    tabs[4] = std::make_unique<IngredientTab>(this);
    tabs[5] = std::make_unique<DrinksTab>(this);
    tabs[6] = std::make_unique<BankDetailTab>(this);
    tabs[7] = std::make_unique<ProviderTab>(this);

    tabs_container->append_page(*tabs[0], "сотрудники");
    tabs_container->append_page(*tabs[1], "продукты");
    tabs_container->append_page(*tabs[2], "приказы");
    tabs_container->append_page(*tabs[3], "закуски");
    tabs_container->append_page(*tabs[4], "ингридиенты");
    tabs_container->append_page(*tabs[5], "напитки");
    tabs_container->append_page(*tabs[6], "реквизиты\nбанка");
    tabs_container->append_page(*tabs[7], "поставщики");
}

void TabManager::select_on_tab(int tab_id, int entry_id) {
    if(tab_id >= tabs.size()){
        return;//TODO выкидывать исключение
    }

    tabs[tab_id]->select_by_id(entry_id);
    tabs_container->set_current_page(tab_id);
}

int TabManager::select_dialog(int tab_id) {
    return tabs[tab_id]->select_dialog();
}
