//
// Created by victor on 05.09.2022.
//

#include "Tab.h"

Tab::Tab(TabManager* tab_manager) : tab_manager(tab_manager) {
    set_orientation(Gtk::ORIENTATION_VERTICAL);

    this->add(*create_top_panel());

    auto scroll = Gtk::make_managed<Gtk::ScrolledWindow>();
    scroll->set_vexpand(true);

    this->add(*scroll);

    listBox = Gtk::make_managed<Gtk::ListBox>();

    scroll->add(*listBox);
}

Gtk::ButtonBox *Tab::create_top_panel() {

    auto top_panel = Gtk::make_managed<Gtk::ButtonBox>();
    top_panel->set_orientation(Gtk::ORIENTATION_HORIZONTAL);

    add_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-add"));
    top_panel->add(*add_button);
    remove_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-delete"));
    top_panel->add(*remove_button);

    return top_panel;
}

Gtk::Button *Tab::getAddButton() const {
    return add_button;
}


Gtk::Button *Tab::getRemoveButton() const {
    return remove_button;
}

Gtk::ListBox *Tab::getListBox() const {
    return listBox;
}

TabManager* Tab::get_tab_manager() const {
    return tab_manager;
}

int Tab::select_dialog() {
    Gtk::ListBox list;

    fill_list(&list);

    list.show_all();

    Gtk::Dialog dialog;
    dialog.add_button("OK", Gtk::RESPONSE_OK);
    dialog.add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::ScrolledWindow scroll;
    scroll.set_vexpand(true);
    scroll.add(list);

    dynamic_cast<Gtk::Container*>(dialog.get_children()[0])->add(scroll);

    scroll.show_all();
    if(dialog.run() == Gtk::RESPONSE_OK){
        auto entry = dynamic_cast<IEntry*>(list.get_selected_row());
        return entry->get_id();
    }

    return -1;
}
