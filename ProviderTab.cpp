//
// Created by victor on 16.09.2022.
//

#include "ProviderTab.h"

#include "Form.h"
#include "gateways/BankDetail/BankDetail.h"
#include "gateways/BankDetail/BankDetailgateway.h"

#include <utility>
#include <iostream>

ProviderTab::ProviderTab(TabManager* manager) : Tab(manager) {

    builder = Gtk::Builder::create_from_file("../provider_menu.glade");

    builder->get_widget("box", info_box);
    builder->get_widget("name_entry", name_entry);
    builder->get_widget("address_entry", address_entry);
    builder->get_widget("entry_number", entry_number);
    builder->get_widget("fax_entry", fax_entry);
    builder->get_widget("email_entry", email_entry);
    builder->get_widget("detail_link", detail_link);
    builder->get_widget("select_button", select_button);
    builder->get_widget("find_button", find_button);

    this->fill_list(getListBox());

    find_button->signal_clicked().connect(sigc::mem_fun(this,&ProviderTab::find_detail));
    select_button->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&ProviderTab::select_deltail,detail_link, get_tab_manager()));

    getListBox()->signal_row_selected().connect(sigc::mem_fun(this,&ProviderTab::select));
}

void ProviderTab::select(Gtk::ListBoxRow* row){
    auto entry = dynamic_cast<Entry*>(row);
    if(entry == nullptr){
        return;
    }

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    //replace box
    if(box->get_children().empty() || box->get_children()[0]->get_name() != "provider_menu"){
        if(!box->get_children().empty()){
            box->remove(*box->get_children()[0]);
        }
        box->add(*info_box);
    }

    name_entry->set_text(entry->get_provider().get_name());
    address_entry->set_text(entry->get_provider().get_post_address());
    entry_number->set_text(entry->get_provider().get_phone_number());
    fax_entry->set_text(entry->get_provider().get_fax());
    email_entry->set_text(entry->get_provider().get_email());

    BankDetailgateway detail_gateway;
    BankDetail detail = detail_gateway.get(entry->get_provider().get_bank_detail());

    detail_link->set_text(detail.getBankName());
    //TODO сделать так во всех выделения памяти
    detail_link->set_data("id",new int(detail.getId()),[](void* data){std::cout << "delete" << std::endl; delete (int*) data;});
}

void ProviderTab::find_detail(){
    int* id = static_cast<int*>(detail_link->get_data("id"));
    get_tab_manager()->select_on_tab(6,*id);
}

void ProviderTab::select_deltail(Gtk::Label* label, TabManager* manager){
    int id = manager->select_dialog(6);
    BankDetailgateway detail_gateway;
    try {
        label->set_text(detail_gateway.get(id).getBankName());
        label->set_data("id", new int(id), [](void* data){std::cout << "delete" << std::endl; delete (int*) data;});
    }catch(std::exception&){}
}

const Provider &ProviderTab::Entry::get_provider() const {
    return provider;
}

ProviderTab::Entry::Entry(Provider provider) : provider(std::move(provider)) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    name_label = Gtk::make_managed<Gtk::Label>(this->provider.get_name());
    post_address_label = Gtk::make_managed<Gtk::Label>(this->provider.get_post_address());
    phone_number_label = Gtk::make_managed<Gtk::Label>(this->provider.get_phone_number());
    fax_label = Gtk::make_managed<Gtk::Label>(this->provider.get_fax());
    email_label = Gtk::make_managed<Gtk::Label>(this->provider.get_email());

    box->add(*name_label);
    box->add(*post_address_label);
    box->add(*phone_number_label);
    box->add(*fax_label);
    box->add(*email_label);

    this->add(*box);
}

int ProviderTab::Entry::get_id() {
    return provider.get_id();
}

// void ProviderTab::select_by_id(int entry_id) {
//
// }

void ProviderTab::fill_list(Gtk::ListBox* list) {
    for(const auto& provider : gateway.get_all()){
        auto entry = Gtk::make_managed<Entry>(provider);
        list->add(*entry);
    }
}

