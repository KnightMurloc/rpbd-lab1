//
// Created by victor on 15.09.2022.
//

#include "BankDetailTab.h"
#include "Form.h"
#include "gateways/BankDetail/BankDetail.h"
#include "gateways/Provider/Provider.h"
#include "gtkmm/dialog.h"
#include "gtkmm/object.h"
#include "sigc++/adaptors/bind.h"
#include "sigc++/functors/mem_fun.h"

#include <string>
#include <utility>
#include <iostream>

BankDetailTab::BankDetailTab(TabManager *manager) : Tab(manager) {
    list = dynamic_cast<EntityList<BankDetail,Entry>*>(create_list());
    set_list(list);

    builder = Gtk::Builder::create_from_file("../bank_detail_menu.glade");

    builder->get_widget("box",info_box);
    builder->get_widget("name_entry",name_entry);
    builder->get_widget("city_entry",city_entry);
    builder->get_widget("tin_entry",tin_entry);
    builder->get_widget("settlement_entry",settlement_entry);
    builder->get_widget("provider_link", provider_link);
    builder->get_widget("select_button", select_button);
    builder->get_widget("find_button", find_button);

    setup_menu(builder);

    auto create_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    create_button->signal_clicked().connect(sigc::mem_fun(this,&BankDetailTab::save_current));
    info_box->add(*create_button);
    info_box->show_all();

    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&BankDetailTab::create));
    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&BankDetailTab::remove_entry));

   list->get_list_box()->signal_row_selected().connect(sigc::mem_fun(this,&BankDetailTab::select));

   find_button->signal_clicked().connect(sigc::mem_fun(this,&BankDetailTab::find_provider));

    select_button->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&BankDetailTab::select_provider,provider_link, get_tab_manager()));

    get_tab_manager()->get_tab(TabName::PROVIDER)->signal_remove().connect(sigc::mem_fun(this,&BankDetailTab::provider_remove_callback));
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

    name_entry->set_text(entry->get_bank_detail()->getBankName());
    city_entry->set_text(entry->get_bank_detail()->getCity());
    tin_entry->set_text(entry->get_bank_detail()->getTin());
    settlement_entry->set_text(entry->get_bank_detail()->getSettlementAccount());
    provider_link->set_text(entry->get_bank_detail()->get_provider()->get_name());
    provider_link->set_data("id",new int(entry->get_bank_detail()->get_provider()->get_id()),[](void* data){delete (int*) data;});
}

void BankDetailTab::save_current(){
   auto entry = dynamic_cast<Entry*>(list->get_selected());
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

   if(provider_link->get_data("id") == nullptr){
       Gtk::MessageDialog message("не указан поставщик");
       message.run();
       return;
    }

   auto detail = entry->get_bank_detail();

   detail->setBankName(name_entry->get_text());
   detail->setCity(city_entry->get_text());
   detail->setTin(tin_entry->get_text());
   detail->setSettlementAccount(settlement_entry->get_text());
   int* p_id = static_cast<int*>(provider_link->get_data("id"));

   detail->set_provider(Provider::get(*p_id));

//    gateway.save(detail);
    BankDetail::save(detail);

   entry->name_label->set_text(detail->getBankName());
   entry->city_label->set_text(detail->getCity());
   entry->tin_label->set_text(tin_entry->get_text());
   entry->account_label->set_text(settlement_entry->get_text().substr(0,10));
}

void BankDetailTab::create(){
    auto builder = Gtk::Builder::create_from_file("../bank_detail_menu.glade");

    setup_menu(builder);

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Label* provider_link_dialog;
    Gtk::Button* select_button_dialog;
    Gtk::Button* find_button_dialog;

    builder->get_widget("provider_link", provider_link_dialog);
    builder->get_widget("select_button", select_button_dialog);
    builder->get_widget("find_button", find_button_dialog);

    find_button_dialog->set_sensitive(false);

    select_button_dialog->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&BankDetailTab::select_provider,provider_link_dialog, get_tab_manager()));

    Gtk::Box* box;
    builder->get_widget("box", box);

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this, dialog, builder](int response){
        if(response == Gtk::RESPONSE_OK){
            Gtk::Entry* name_entry_dialog;
            Gtk::Entry* city_entry_dialog;
            Gtk::Entry* tin_entry_dialog;
            Gtk::Entry* settlement_entry_dialog;
            Gtk::Label* provider_link_dialog;


            builder->get_widget("name_entry",name_entry_dialog);
            builder->get_widget("city_entry",city_entry_dialog);
            builder->get_widget("tin_entry",tin_entry_dialog);
            builder->get_widget("settlement_entry",settlement_entry_dialog);

            builder->get_widget("provider_link", provider_link_dialog);


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

            if(provider_link_dialog->get_data("id") == nullptr){
                Gtk::MessageDialog message("не указан поставщик");
                message.run();
                return;
            }

//             auto detail = gateway.create(
//               name_entry_dialog->get_text(),
//               city_entry_dialog->get_text(),
//               tin_entry_dialog->get_text(),
//               settlement_entry_dialog->get_text()
//             );
            int* p_id = static_cast<int*>(provider_link_dialog->get_data("id"));
            auto detail = BankDetail::create(
                name_entry_dialog->get_text(),
                city_entry_dialog->get_text(),
                tin_entry_dialog->get_text(),
                settlement_entry_dialog->get_text(),
                Provider::get(*p_id)
            );

            auto entry = Gtk::make_managed<Entry>(detail);

            list->add_entity(entry);

           list->show_all();
        }
        dialog->close();
        delete dialog;
    });

    dialog->show_all();
}

void BankDetailTab::remove_entry(){
   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry == nullptr){
       return;
   }

   on_remove.emit(entry->get_bank_detail());
//    gateway.remove(entry->get_bank_detail());
    BankDetail::remove(entry->get_bank_detail());

   Gtk::Box* box;
   Form::getInstance().getBuilder()->get_widget("info_box", box);

   box->remove(*box->get_children()[0]);

   list->remove_entity(entry);
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
    return detail->get_id();
}

BankDetailTab::Entry::Entry(std::shared_ptr<BankDetail> detail) : detail(detail) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);
    name_label = Gtk::make_managed<Gtk::Label>(this->detail->getBankName());
    city_label = Gtk::make_managed<Gtk::Label>(this->detail->getCity());
    tin_label = Gtk::make_managed<Gtk::Label>(this->detail->getTin());
    account_label = Gtk::make_managed<Gtk::Label>(this->detail->getSettlementAccount());

    box->add(*name_label);
    box->add(*city_label);
    box->add(*tin_label);
    box->add(*account_label);

    this->add(*box);
}

std::shared_ptr<BankDetail> BankDetailTab::Entry::get_bank_detail(){
    return detail;
}

IList* BankDetailTab::create_list(){
    auto list = Gtk::make_managed<EntityList<BankDetail,Entry>>();

    list->add_column_title("название");
    list->add_column_title("город");
    list->add_column_title("ИНН");
    list->add_column_title("расчётный счёт");

    return list;
}

void BankDetailTab::find_provider(){
    int* id = static_cast<int*>(provider_link->get_data("id"));
    get_tab_manager()->select_on_tab(TabName::PROVIDER,*id);
}

void BankDetailTab::select_provider(Gtk::Label* label, TabManager* manager){
    int id = manager->select_dialog(TabName::PROVIDER);
    if(id == -1){
        fmt::print("not found\n");
        return;
    }
    label->set_text(Provider::get(id)->get_name());
    label->set_data("id", new int(id), [](void* data){std::cout << "delete" << std::endl; delete (int*) data;});
//     BankDetailgateway detail_gateway;
//     try {
//         label->set_text(detail_gateway.get(id)->getBankName());
//         label->set_data("id", new int(id), [](void* data){std::cout << "delete" << std::endl; delete (int*) data;});
//     }catch(std::exception&){
//         fmt::print("not exception\n");
//     }
}

void BankDetailTab::provider_remove_callback(std::shared_ptr<IEntity> entity){
//     auto waiter = std::dynamic_pointer_cast<Employeer>(entity);
    if(entity == nullptr){
        return;
    }

    for(auto child : list->get_list_box()->get_children()){
        auto entry = dynamic_cast<Entry*>(child);
        if(entry->get_bank_detail()->get_provider()->get_id() == entity->get_id()){
            list->remove_entity(entry);
        }
    }
}
