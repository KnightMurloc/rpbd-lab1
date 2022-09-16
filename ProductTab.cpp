//
// Created by victor on 05.09.2022.
//

#include "ProductTab.h"

ProductTab::ProductTab(TabManager* tab_manager) : Tab(tab_manager)  {
    auto test_row = Gtk::make_managed<Gtk::Label>("Products");
    getListBox()->append(*test_row);
}

void ProductTab::save_current() {

}

void ProductTab::cancel_current() {

}

// void ProductTab::select_by_id(int entry_id) {
//     //TODO
// }

void ProductTab::fill_list(Gtk::ListBox* list) {

}

//int ProductTab::select_dialog() {
//    return 0;
//}
