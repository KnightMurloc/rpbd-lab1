//
// Created by victor on 05.09.2022.
//

#ifndef LAB1_PRODUCTTAB_H
#define LAB1_PRODUCTTAB_H

#include <gtkmm.h>
#include "Tab.h"

class ProductTab : public Tab {
private:
protected:
    void fill_list(Gtk::ListBox* list) override;

public:
    ProductTab(TabManager* tab_manager);

    void save_current();

    void cancel_current();

    void select_by_id(int entry_id) override;

//    int select_dialog() override;
};


#endif //LAB1_PRODUCTTAB_H
