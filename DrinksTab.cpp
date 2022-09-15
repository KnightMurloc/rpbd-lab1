//
// Created by victor on 15.09.2022.
//

#include "DrinksTab.h"

int DrinksTab::Entry::get_id() {
    return drink.getId();
}

DrinksTab::Entry::Entry(const Drink &drink) : drink(drink) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    name_label = Gtk::make_managed<Gtk::Label>(drink.getName());
    strength_label = Gtk::make_managed<Gtk::Label>(std::to_string(drink.getStrength()));
    size_label = Gtk::make_managed<Gtk::Label>(std::to_string(drink.getSize()));
    container_label = Gtk::make_managed<Gtk::Label>(drink.getContainer());

    box->add(*name_label);
    box->add(*strength_label);
    box->add(*size_label);
    box->add(*container_label);

    this->add(*box);
}

const Drink &DrinksTab::Entry::getDrink() const {
    return drink;
}

void DrinksTab::select_by_id(int entry_id) {

}

void DrinksTab::fill_list(Gtk::ListBox *list) {
    for(const auto& drink : gateway.get_all()){
        auto entry = Gtk::make_managed<Entry>(drink);

        list->append(*entry);
    }
}

DrinksTab::DrinksTab(TabManager *manager) : Tab(manager) {
    fill_list(getListBox());
}
