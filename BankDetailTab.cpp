//
// Created by victor on 15.09.2022.
//

#include "BankDetailTab.h"
#include "Form.h"
#include "gateways/BankDetail/BankDetail.h"
#include "gtkmm/dialog.h"

#include <string>
#include <utility>
#include <iostream>

BankDetailTab::BankDetailTab(TabManager *manager) : Tab(manager) {
    this->fill_list(getListBox());

    builder = Gtk::Builder::create_from_file("../bank_detail_menu.glade");

    builder->get_widget("box",info_box);
    builder->get_widget("name_entry",name_entry);
    builder->get_widget("city_entry",city_entry);
    builder->get_widget("tin_entry",tin_entry);
    builder->get_widget("settlement_entry",settlement_entry);

    setup_menu(builder);

    auto create_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    create_button->signal_clicked().connect(sigc::mem_fun(this,&BankDetailTab::save_current));
    info_box->add(*create_button);
    info_box->show_all();

    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&BankDetailTab::create));
    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&BankDetailTab::remove_entry));

    getListBox()->signal_row_selected().connect(sigc::mem_fun(this,&BankDetailTab::select));

    add_clumn_lable("название");
    add_clumn_lable("город");
    add_clumn_lable("ИНН");
    add_clumn_lable("расчётный счёт");
}

void BankDetailTab::select(Gtk::ListBoxRow* row){
    auto entry = dynamic_cast<Entry*>(row);
    if(entry == nullptr){
        return;
    }

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    //replace box
    if(box->get_children().empty() || box->get_children()[0]->get_name() != "bamd_detail_menu"){
        if(!box->get_children().empty()){
            box->remove(*box->get_children()[0]);
        }
        box->add(*info_box);
    }

    name_entry->set_text(entry->get_bank_detail().getBankName());
    city_entry->set_text(entry->get_bank_detail().getCity());
    tin_entry->set_text(entry->get_bank_detail().getTin());
    settlement_entry->set_text(entry->get_bank_detail().getSettlementAccount());
}

void BankDetailTab::save_current(){
    auto entry = dynamic_cast<Entry*>(getListBox()->get_selected_row());
    if(entry == nullptr){
        return;
    }

    if(name_entry->get_text().empty()){
        Gtk::MessageDialog message("не указано имя");
        message.run();
        return;
    }

    if(city_entry->get_text().empty()){
        Gtk::MessageDialog message("не указан город");
        message.run();
        return;
    }

    if(tin_entry->get_text().length() != 10){
        Gtk::MessageDialog message("не коректный ИНН");
        message.run();
        return;
    }

    if(settlement_entry->get_text().length() != 20){
        Gtk::MessageDialog message("не коректный расчётный счёт");
        message.run();
        return;
    }

    auto& detail = entry->get_bank_detail();

    detail.setBankName(name_entry->get_text());
    detail.setCity(city_entry->get_text());
    detail.setTin(tin_entry->get_text());
    detail.setSettlementAccount(settlement_entry->get_text());

    gateway.save(detail);

    entry->name_label->set_text(detail.getBankName());
    entry->city_label->set_text(detail.getCity());
    entry->tin_label->set_text(tin_entry->get_text());
    entry->account_label->set_text(settlement_entry->get_text().substr(0,10));
}

void BankDetailTab::create(){
    auto builder = Gtk::Builder::create_from_file("../bank_detail_menu.glade");

    setup_menu(builder);

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Box* box;
    builder->get_widget("box", box);

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this, dialog, builder](int response){
        if(response == Gtk::RESPONSE_OK){
            Gtk::Entry* name_entry_dialog;
            Gtk::Entry* city_entry_dialog;
            Gtk::Entry* tin_entry_dialog;
            Gtk::Entry* settlement_entry_dialog;

            builder->get_widget("name_entry",name_entry_dialog);
            builder->get_widget("city_entry",city_entry_dialog);
            builder->get_widget("tin_entry",tin_entry_dialog);
            builder->get_widget("settlement_entry",settlement_entry_dialog);

            if(name_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указано имя");
                message.run();
                return;
            }

            if(city_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указан город");
                message.run();
                return;
            }

            if(tin_entry_dialog->get_text().length() != 10){
                Gtk::MessageDialog message("не коректный ИНН");
                message.run();
                return;
            }

            if(settlement_entry_dialog->get_text().length() != 20){
                Gtk::MessageDialog message("не коректный расчётный счёт");
                message.run();
                return;
            }

            BankDetail detail = gateway.create(
              name_entry_dialog->get_text(),
              city_entry_dialog->get_text(),
              tin_entry_dialog->get_text(),
              settlement_entry_dialog->get_text()
            );

            auto entry = Gtk::make_managed<Entry>(detail);

            getListBox()->append(*entry);
            getListBox()->show_all();
        }
        dialog->close();
        delete dialog;
    });

    dialog->show_all();
}

void BankDetailTab::remove_entry(){
    auto entry = dynamic_cast<Entry*>(getListBox()->get_selected_row());
    if(entry == nullptr){
        return;
    }

    gateway.remove(entry->get_bank_detail());

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    box->remove(*box->get_children()[0]);

    getListBox()->remove(*entry);
}

void BankDetailTab::setup_menu(Glib::RefPtr<Gtk::Builder> builder){

    Gtk::Entry* tin_entry_menu;
    Gtk::Entry* settlement_entry_menu;

    builder->get_widget("tin_entry",tin_entry_menu);
    builder->get_widget("settlement_entry",settlement_entry_menu);

    tin_entry_menu->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,tin_entry_menu));
    settlement_entry_menu->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,settlement_entry_menu));
}

int BankDetailTab::Entry::get_id() {
    return detail.getId();
}

BankDetailTab::Entry::Entry(BankDetail detail) : detail(detail) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);
    name_label = Gtk::make_managed<Gtk::Label>(this->detail.getBankName());
    city_label = Gtk::make_managed<Gtk::Label>(this->detail.getCity());
    tin_label = Gtk::make_managed<Gtk::Label>(this->detail.getTin());
    account_label = Gtk::make_managed<Gtk::Label>(this->detail.getSettlementAccount());

    box->add(*name_label);
    box->add(*city_label);
    box->add(*tin_label);
    box->add(*account_label);

    this->add(*box);
}

// void BankDetailTab::select_by_id(int entry_id) {
// //    fill_list(getListBox());
// }

void BankDetailTab::fill_list(Gtk::ListBox *list) {
    for(auto& data : gateway.get_all()){
        auto entry = Gtk::make_managed<Entry>(data);
        list->append(*entry);
    }

}

BankDetail& BankDetailTab::Entry::get_bank_detail(){
    return detail;
}
