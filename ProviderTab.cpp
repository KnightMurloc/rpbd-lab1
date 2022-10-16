//
// Created by victor on 16.09.2022.
//

#include "ProviderTab.h"

#include "Form.h"
#include "TabManager.h"
#include "fmt/core.h"
#include "gateways/BankDetail/BankDetail.h"
#include "gateways/BankDetail/BankDetailgateway.h"
#include "gateways/Gateway.h"
#include "gateways/Provider/Provider.h"
#include "gtkmm/dialog.h"
#include "sigc++/adaptors/bind.h"
#include "sigc++/functors/mem_fun.h"

#include <utility>
#include <iostream>

ProviderTab::ProviderTab(TabManager* manager) : Tab(manager) {

    list = dynamic_cast<EntityList<Provider,Entry>*>(create_list());
    set_list(list);

    builder = Gtk::Builder::create_from_file("../provider_menu.glade");

    builder->get_widget("box", info_box);
    builder->get_widget("name_entry", name_entry);
    builder->get_widget("address_entry", address_entry);
    builder->get_widget("entry_number", entry_number);
    builder->get_widget("fax_entry", fax_entry);
    builder->get_widget("email_entry", email_entry);
//     builder->get_widget("detail_link", detail_link);
//     builder->get_widget("select_button", select_button);
//     builder->get_widget("find_button", find_button);

//     setup_menu(builder);

    auto save_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    save_button->signal_clicked().connect(sigc::mem_fun(this,&ProviderTab::save_current));
    info_box->add(*save_button);
    info_box->show_all();

//     find_button->signal_clicked().connect(sigc::mem_fun(this,&ProviderTab::find_detail));
//     select_button->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&ProviderTab::select_deltail,detail_link, get_tab_manager()));

    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&ProviderTab::remove_entry));

    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&ProviderTab::create));

   list->get_list_box()->signal_row_selected().connect(sigc::mem_fun(this,&ProviderTab::select));

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

    name_entry->set_text(entry->get_provider()->get_name());
    address_entry->set_text(entry->get_provider()->get_post_address());
    entry_number->set_text(entry->get_provider()->get_phone_number());
    fax_entry->set_text(entry->get_provider()->get_fax());
    email_entry->set_text(entry->get_provider()->get_email());

//     try{
//         std::shared_ptr<BankDetail> detail = entry->get_provider()->get_bank_detail();
//         detail_link->set_text(detail->getBankName());
//
//         detail_link->set_data("id",new int(detail->get_id()),[](void* data){delete (int*) data;});
//         find_button->set_sensitive(true);
//     }catch(GatewayException& e){
//         detail_link->set_text("none");
//         detail_link->set_data("id",nullptr,[](void* data){delete (int*) data;});
//         find_button->set_sensitive(false);
//
//     }

}

void ProviderTab::save_current(){
   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry == nullptr){
       return;
   }

   auto provider = entry->get_provider();

   if(name_entry->get_text().empty()){
       Gtk::MessageDialog message("не указано имя");
       message.run();
       return;
   }

   if(address_entry->get_text().empty()){
       Gtk::MessageDialog message("не указан почтовый адрес");
       message.run();
       return;
   }

   if(entry_number->get_text().empty()){
       Gtk::MessageDialog message("не указан номер телефона");
       message.run();
       return;
   }

   if(fax_entry->get_text().empty()){
       Gtk::MessageDialog message("не указан факс");
       message.run();
       return;
   }

   if(email_entry->get_text().empty()){
       Gtk::MessageDialog message("не указан email");
       message.run();
       return;
   }

   provider->set_name(name_entry->get_text());
   provider->set_post_address(address_entry->get_text());
   provider->set_phone_number(entry_number->get_text());
   provider->set_fax(fax_entry->get_text());
   provider->set_email(email_entry->get_text());
//    int* d_id = static_cast<int*>(detail_link->get_data("id"));

//    if(d_id)
//        provider->set_bank_detail(BankDetail::get(*d_id));

    Provider::save(provider);

//    gateway.save(provider);


   entry->name_label->set_text(provider->get_name());
   entry->post_address_label->set_text(provider->get_post_address());
   entry->phone_number_label->set_text(provider->get_phone_number());
   entry->fax_label->set_text(provider->get_fax());
   entry->email_label->set_text(provider->get_email());
}

void ProviderTab::create(){
    auto builder = Gtk::Builder::create_from_file("../provider_menu.glade");
//     setup_menu(builder);

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Box* box;
//     Gtk::Label* detail_link_dialog;
//     Gtk::Button* select_button_dialog;
//     Gtk::Button* find_button_dialog;

    builder->get_widget("box", box);
//     builder->get_widget("detail_link", detail_link_dialog);
//     builder->get_widget("select_button", select_button_dialog);
//     builder->get_widget("find_button", find_button_dialog);
//     find_button_dialog->set_sensitive(false);
    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);
//     select_button_dialog->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&ProviderTab::select_deltail,detail_link_dialog, get_tab_manager()));

    dialog->signal_response().connect([this,dialog, builder](int response){
        if(response == Gtk::RESPONSE_OK){
            Gtk::Entry* name_entry_dialog;
            Gtk::Entry* address_entry_dialog;
            Gtk::Entry* entry_number_dialog;
            Gtk::Entry* fax_entry_dialog;
            Gtk::Entry* email_entry_dialog;
//             Gtk::Label* detail_link_dialog;

            builder->get_widget("name_entry", name_entry_dialog);
            builder->get_widget("address_entry", address_entry_dialog);
            builder->get_widget("entry_number", entry_number_dialog);
            builder->get_widget("fax_entry", fax_entry_dialog);
            builder->get_widget("email_entry", email_entry_dialog);
//             builder->get_widget("detail_link", detail_link_dialog);

            if(name_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указано имя");
                message.run();
                return;
            }

            if(address_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указан почтовый адрес");
                message.run();
                return;
            }

            if(entry_number_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указан номер телефона");
                message.run();
                return;
            }

            if(fax_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указан факс");
                message.run();
                return;
            }

            if(email_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указан email");
                message.run();
                return;
            }

//             int* d_id_ptr = static_cast<int*>(detail_link_dialog->get_data("id"));

//             int d_id;
//             if(d_id_ptr == nullptr){
//                 d_id = -1;
//             }else{
//                 d_id = *d_id_ptr;
//             }
//             fmt::print("create provider {}\n",*d_id_ptr);

//             auto provider = gateway.create(
//               name_entry_dialog->get_text(),
//               address_entry_dialog->get_text(),
//               entry_number_dialog->get_text(),
//               fax_entry_dialog->get_text(),
//               email_entry_dialog->get_text(),
//               d_id
//             );
            auto provider = Provider::create(
              name_entry_dialog->get_text(),
              address_entry_dialog->get_text(),
              entry_number_dialog->get_text(),
              fax_entry_dialog->get_text(),
              email_entry_dialog->get_text()
//               d_id
            );

            auto entry = Gtk::make_managed<Entry>(provider);
        }

        dialog->close();
        delete dialog;
    });

    dialog->show_all();
}

void ProviderTab::remove_entry(){
   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry == nullptr){
       return;
   }

   on_remove.emit(entry->get_provider());

   Provider::remove(entry->get_provider());
//    gateway.remove(entry->get_provider());

   Gtk::Box* box;
   Form::getInstance().getBuilder()->get_widget("info_box", box);

   box->remove(*box->get_children()[0]);

   list->remove_entity(entry);

//    get_tab_manager()->remove_on_tab(TabName::BACK_DETAIL, entry->get_provider()->get_bank_detail()->get_id());
}

// void ProviderTab::setup_menu(Glib::RefPtr<Gtk::Builder> builder){
//     Gtk::Entry* entry_number_menu;
//     Gtk::Entry* fax_entry_menu;
//
//     builder->get_widget("entry_number", entry_number_menu);
//     builder->get_widget("fax_entry", fax_entry_menu);
//
//     entry_number_menu->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,entry_number_menu));
//
//     fax_entry_menu->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,fax_entry_menu));
// }

// void ProviderTab::find_detail(){
//     int* id = static_cast<int*>(detail_link->get_data("id"));
//     get_tab_manager()->select_on_tab(TabName::BACK_DETAIL,*id);
// }

// void ProviderTab::select_deltail(Gtk::Label* label, TabManager* manager){
//     int id = manager->select_dialog(TabName::BACK_DETAIL);
//     if(id == -1){
//         fmt::print("not found\n");
//         return;
//     }
//     BankDetailgateway detail_gateway;
//     try {
//         label->set_text(detail_gateway.get(id)->getBankName());
//         label->set_data("id", new int(id), [](void* data){std::cout << "delete" << std::endl; delete (int*) data;});
//     }catch(std::exception&){
//         fmt::print("not exception\n");
//     }
// }

std::shared_ptr<Provider> ProviderTab::Entry::get_provider() {
    return provider;
}

ProviderTab::Entry::Entry(std::shared_ptr<Provider> provider) : provider(provider) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    name_label = Gtk::make_managed<Gtk::Label>(this->provider->get_name());
    post_address_label = Gtk::make_managed<Gtk::Label>(this->provider->get_post_address());
    phone_number_label = Gtk::make_managed<Gtk::Label>(this->provider->get_phone_number());
    fax_label = Gtk::make_managed<Gtk::Label>(this->provider->get_fax());
    email_label = Gtk::make_managed<Gtk::Label>(this->provider->get_email());

    box->add(*name_label);
    box->add(*post_address_label);
    box->add(*phone_number_label);
    box->add(*fax_label);
    box->add(*email_label);

    this->add(*box);
}

int ProviderTab::Entry::get_id() {
    return provider->get_id();
}

IList* ProviderTab::create_list(){

    auto list = Gtk::make_managed<EntityList<Provider,Entry>>();

    list->add_column_title("название");
    list->add_column_title("почтовый адрес");
    list->add_column_title("телефон");
    list->add_column_title("факс");
    list->add_column_title("email");

    return list;
}
