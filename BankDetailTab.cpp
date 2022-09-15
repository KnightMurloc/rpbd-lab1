//
// Created by victor on 15.09.2022.
//

#include "BankDetailTab.h"

#include <utility>
#include <iostream>

int BankDetailTab::Entry::get_id() {
    return detail.getId();
}

BankDetailTab::Entry::Entry(BankDetail detail) : detail(detail) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);
    name_label = Gtk::make_managed<Gtk::Label>(this->detail.getBankName());
    city_label = Gtk::make_managed<Gtk::Label>(this->detail.getCity());
    tin_label = Gtk::make_managed<Gtk::Label>(std::to_string(this->detail.getTin()));
    account_label = Gtk::make_managed<Gtk::Label>(std::to_string(this->detail.getSettlementAccount()));

    box->add(*name_label);
    box->add(*city_label);
    box->add(*tin_label);
    box->add(*account_label);

    this->add(*box);
}

BankDetailTab::BankDetailTab(TabManager *manager) : Tab(manager) {
    this->fill_list(getListBox());
}

void BankDetailTab::select_by_id(int entry_id) {
//    fill_list(getListBox());
}

void BankDetailTab::fill_list(Gtk::ListBox *list) {
    for(auto& data : gateway.get_all()){
        auto entry = Gtk::make_managed<Entry>(data);
        auto test = Gtk::make_managed<Gtk::Label>("test");
//        list->add(*test);
        list->append(*entry);
    }

}
