//
// Created by victor on 05.09.2022.
//

#include "EmployeesTab.h"
#include "Form.h"
#include <iostream>
#include <fmt/format.h>
#include <memory>
#include "gateways/Employeer/EmployeerGateway.h"
#include "gateways/Orders/OrderGateway.h"
#include "gtkmm/enums.h"
#include "sigc++/adaptors/bind.h"
#include "sigc++/functors/mem_fun.h"

EmployeesTab::EmployeesTab(TabManager* tab_manager) : Tab(tab_manager) {

    list = std::make_unique<EntityList<Employeer,Entry>>(&gateway);
    set_list(list.get());
    list->show_all();

//    current_search = std::make_unique<DefaultSearch>(&gateway);

    builder = Gtk::Builder::create_from_file("../employeer_menu.glade");

    builder->get_widget("employees_info", info_box);
    builder->get_widget("first_name_entry", first_name_entry);
    builder->get_widget("last_name_entry", last_name_entry);
    builder->get_widget("oreder_link", order_link);
    builder->get_widget("emp_find_button", find_button);
    builder->get_widget("emp_select_button", select_button);
    builder->get_widget("patronymic_entry", patronymic_entry);
    builder->get_widget("address_entry", address_entry);
    builder->get_widget("day_entry", day_entry);
    builder->get_widget("month_entry", month_entry);
    builder->get_widget("year_entry", year_entry);
    builder->get_widget("salary_entry", salary_entry);
    builder->get_widget("post_combobox", post_combobox);

    setup_menu(builder);

    auto save_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    save_button->signal_clicked().connect(sigc::mem_fun(this,&EmployeesTab::save_current));
    info_box->add(*save_button);
    info_box->show_all();

    find_button->signal_clicked().connect(sigc::mem_fun(this,&EmployeesTab::find_order));
//    select_button->signal_clicked().connect(sigc::mem_fun<Gtk::Label*>(this,&EmployeesTab::select_order,oreder_link));
    select_button->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&EmployeesTab::select_order, order_link, get_tab_manager()));

    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&EmployeesTab::create));
    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&EmployeesTab::remove));

   list->get_search_entry()->signal_activate().connect(sigc::bind<EntityList<Employeer,Entry>*>(&EmployeesTab::search,list.get()));
//    stop_search->signal_clicked().connect(sigc::mem_fun(this,&EmployeesTab::search_stop));

//    fill_list(getListBox());
//     scroll->signal_edge_reached().connect(sigc::mem_fun(this,&EmployeesTab::scroll_event));

//     list->set_select_callback(&EmployeesTab::select);

//    getListBox()->signal_row_selected().connect(sigc::mem_fun(this,&EmployeesTab::select));
    list->get_list_box()->signal_row_selected().connect(sigc::mem_fun(this,&EmployeesTab::select));


    add_clumn_lable("имя");
    add_clumn_lable("фамилия");
    add_clumn_lable("отчество");
    add_clumn_lable("адрес");
    add_clumn_lable("дата рождения");
    add_clumn_lable("зарплата");
    add_clumn_lable("должность");
}

//TODO устанавливать gateway в листе
void EmployeesTab::search(EntityList<Employeer,Entry>* list){


   std::string str = list->get_search_text();
   std:: transform(str.begin(), str.end(), str.begin(), ::tolower);
   auto gateway = dynamic_cast<EmployeerGateway*>(list->get_gateway());
   if(gateway == nullptr){
       return;
    }
   auto ptr = std::make_unique<NameSearch>(gateway,str );
   list->set_search(std::move(ptr));
//    fill_list(getListBox());
//    getListBox()->show_all();

}

void EmployeesTab::search_stop(){
//    current_search = std::make_unique<DefaultSearch>(&gateway);
//    fill_list(getListBox());
//    getListBox()->show_all();
}

void EmployeesTab::select(Gtk::ListBoxRow *entry_row) {
    auto entry = dynamic_cast<Entry*>(entry_row);
    if(entry == nullptr){
        return;
    }

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    //replace box
    if(box->get_children().empty() || box->get_children()[0]->get_name() != "employees_info"){
        if(!box->get_children().empty()){
            box->remove(*box->get_children()[0]);
        }
        box->add(*info_box);
    }
    first_name_entry->set_text(entry->get_emp()->getFirstName());
    last_name_entry->set_text(entry->get_emp()->getLastName());
    if(entry->get_emp()->get_movement_id() != -1){
        try {
            order_link->set_text(std::to_string(entry->get_emp()->get_movement()->get_order_number()));
            order_link->set_data("id", new int(entry->get_emp()->get_movement()->get_id()), [](void* data){delete (int*) data;});
            find_button->set_sensitive(true);
        } catch (GatewayException&) {
            order_link->set_text("none");
            find_button->set_sensitive(false);
        }
    }else{
        order_link->set_text("none");
        find_button->set_sensitive(false);
    }

    patronymic_entry->set_text(entry->get_emp()->getPatronymic());
    address_entry->set_text(entry->get_emp()->getAddress());
    tm date{};
    strptime(entry->get_emp()->getBirthDate().c_str(), "%Y-%m-%d", &date);
    day_entry->set_text(std::to_string(date.tm_mday));
    month_entry->set_text(std::to_string(date.tm_mon + 1));
    year_entry->set_text(std::to_string(date.tm_year + 1900));
    salary_entry->set_text(fmt::format("{}",entry->get_emp()->getSalary()));
    post_combobox->set_active_id(entry->get_emp()->getPostAsString());
}

void EmployeesTab::save_current() {
   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry == nullptr){
       return;
   }

   auto empl = entry->get_emp();

   if(first_name_entry->get_text().empty()){
       Gtk::MessageDialog message("не указано имя");
       message.run();
       return;
   }

   if(last_name_entry->get_text().empty()){
       Gtk::MessageDialog message("не указана фамилия");
       message.run();
       return;
   }
   if(address_entry->get_text().empty()){
       Gtk::MessageDialog message("не указан адрес");
       message.run();
       return;
   }
   int day;
   int month;
   int year;
   float salary;
   try {
       day = std::stoi(day_entry->get_text());
       month = std::stoi(month_entry->get_text());
       year = std::stoi(year_entry->get_text());
   } catch (std::exception&) {
       Gtk::MessageDialog message("не коректная дата");
       message.run();
       return;
   }

   if(!check_date(day,month,year)){
       Gtk::MessageDialog message("не коректная дата");
       message.run();
       return;
   }

   try{
       salary = std::stof(salary_entry->get_text());
   } catch (std::exception&) {
       Gtk::MessageDialog message("не коректная зарплата");
       message.run();
       return;
   }

   empl->setFirstName(first_name_entry->get_text());
   empl->setLastName(last_name_entry->get_text());
   empl->setPatronymic(patronymic_entry->get_text());
   empl->setAddress(address_entry->get_text());
   empl->setSalary(salary);
   empl->setBirthDate(fmt::format("{}-{}-{}",year,month,day));
   empl->setPost(string_to_post(post_combobox->get_active_id()));

   if(order_link->get_text() != "none") {
       int* movement_id = static_cast<int*>(order_link->get_data("id"));
       empl->set_movement_id(*movement_id);
   }

   gateway.save(empl);

   entry->first_name->set_text(empl->getFirstName());
   entry->last_name->set_text(empl->getLastName());
   entry->patronymic->set_text(empl->getPatronymic());
   entry->address->set_text(empl->getAddress());
   entry->birth_date->set_text(empl->getBirthDate());
   entry->salary->set_text(std::to_string(empl->getSalary()));
   entry->post->set_text(empl->getPostAsString());
}

void EmployeesTab::cancel_current() {
    std::cout << "cancel EmployeesTab" << std::endl;
}

void EmployeesTab::find_order() {
   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry == nullptr){
       return;
   }
//        this->get_tab_manager()->select_on_tab(1, entry->getEmp().getId());
   get_tab_manager()->select_on_tab(TabName::ORDER, entry->get_emp()->get_movement_id());
}

void EmployeesTab::select_order(Gtk::Label* label, TabManager* manager) {
    int id = manager->select_dialog(TabName::ORDER);
    if(id == -1){
        return;
    }
    OrderGateway orderGateway;
    try {
        label->set_text(std::to_string(orderGateway.get(id)->get_order_number()));
        label->set_data("id", new int(id), [](void* data){delete (int*) data;});
    }catch(std::exception&){}
}

void EmployeesTab::setup_menu(Glib::RefPtr<Gtk::Builder> builder) {
    Gtk::Entry* day_entry;
    Gtk::Entry* month_entry;
    Gtk::Entry* year_entry;
    Gtk::Entry* salary_entry;
    builder->get_widget("day_entry", day_entry);
    builder->get_widget("month_entry", month_entry);
    builder->get_widget("year_entry", year_entry);
    builder->get_widget("salary_entry", salary_entry);
    day_entry->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,day_entry));
    month_entry->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,month_entry));
    year_entry->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,year_entry));
    salary_entry->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::float_only,salary_entry));

}

void EmployeesTab::create() {
    auto builder = Gtk::Builder::create_from_file("../employeer_menu.glade");

    setup_menu(builder);

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Box* box;
    builder->get_widget("employees_info", box);

    Gtk::Label* order_link_dialog;
    Gtk::Button* find_button_dialog;
    Gtk::Button* select_button_dialog;

    builder->get_widget("oreder_link", order_link_dialog);
    builder->get_widget("emp_find_button", find_button_dialog);
    builder->get_widget("emp_select_button", select_button_dialog);

    find_button_dialog->set_sensitive(false);
//            select_button_dialog->signal_clicked().connect(sigc::bind<Gtk::Label*>(&EmployeesTab::select_order,oreder_link_dialog));
    select_button_dialog->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&EmployeesTab::select_order, order_link_dialog, get_tab_manager()));

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this, dialog, builder](int response){
        if(response == Gtk::RESPONSE_OK) {
            Gtk::Entry* first_name_entry_dialog;
            Gtk::Entry* last_name_entry_dialog;
            Gtk::Label* order_link_dialog;
//             Gtk::Button* find_button_dialog;
//             Gtk::Button* select_button_dialog;
            Gtk::Entry* patronymic_entry_dialog;
            Gtk::Entry* address_entry_dialog;
            Gtk::Entry* day_entry_dialog;
            Gtk::Entry* month_entry_dialog;
            Gtk::Entry* year_entry_dialog;
            Gtk::Entry* salary_entry_dialog;
            Gtk::ComboBoxText* post_combobox_dialog;

            builder->get_widget("first_name_entry", first_name_entry_dialog);
            builder->get_widget("last_name_entry", last_name_entry_dialog);
            builder->get_widget("oreder_link", order_link_dialog);
//             builder->get_widget("emp_find_button", find_button_dialog);
//             builder->get_widget("emp_select_button", select_button_dialog);
            builder->get_widget("patronymic_entry", patronymic_entry_dialog);
            builder->get_widget("address_entry", address_entry_dialog);
            builder->get_widget("day_entry", day_entry_dialog);
            builder->get_widget("month_entry", month_entry_dialog);
            builder->get_widget("year_entry", year_entry_dialog);
            builder->get_widget("salary_entry", salary_entry_dialog);
            builder->get_widget("post_combobox", post_combobox_dialog);

//             find_button_dialog->set_sensitive(false);
// //            select_button_dialog->signal_clicked().connect(sigc::bind<Gtk::Label*>(&EmployeesTab::select_order,oreder_link_dialog));
//             select_button_dialog->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&EmployeesTab::select_order, order_link_dialog, get_tab_manager()));


            if(first_name_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указано имя");
                message.run();
                return;
            }

            if(last_name_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указана фамилия");
                message.run();
                return;
            }
            if(address_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указан адрес");
                message.run();
                return;
            }
            int day;
            int month;
            int year;
            float salary;
            try {
                day = std::stoi(day_entry_dialog->get_text());
                month = std::stoi(month_entry_dialog->get_text());
                year = std::stoi(year_entry_dialog->get_text());
            } catch (std::exception&) {
                Gtk::MessageDialog message("не коректная дата");
                message.run();
                return;
            }

            if(!check_date(day,month,year)){
                Gtk::MessageDialog message("не коректная дата");
                message.run();
                return;
            }

            try{
                salary = std::stof(salary_entry_dialog->get_text());
            } catch (std::exception&) {
                Gtk::MessageDialog message("не коректная зарплата");
                message.run();
                return;
            }
            int movement_id;
            if(order_link_dialog->get_text() != "none") {
                movement_id = *static_cast<int*>(order_link_dialog->get_data("id"));
            }else{
                movement_id = -1;
            }

            auto empl = gateway.create(
                    first_name_entry_dialog->get_text(),
                    last_name_entry_dialog->get_text(),
                    patronymic_entry_dialog->get_text(),
                    address_entry_dialog->get_text(),
                    fmt::format("{}-{}-{}",year,month,day),
                    salary,
                    movement_id,
                    string_to_post(post_combobox_dialog->get_active_id()));

            auto entry = Gtk::make_managed<Entry>(empl);
           list->add_entity(entry);
           list->show_all();

            dialog->close();
            delete dialog;
        }else{
            dialog->close();
            delete dialog;
        }
    });

    dialog->show_all();
}

void EmployeesTab::remove() {
   auto entry = dynamic_cast<Entry*>(list->get_selected());
   if(entry == nullptr){
       return;
   }

   gateway.remove(entry->get_emp());

   Gtk::Box* box;
   Form::getInstance().getBuilder()->get_widget("info_box", box);

   box->remove(*box->get_children()[0]);

   list->remove_entity(entry);
}

void EmployeesTab::fill_list(Gtk::ListBox* list) {

//    for(auto child : getListBox()->get_children()){
//        getListBox()->remove(*child);
//    }
//
//    auto data = current_search->get_great_then(0,20);
//    for(auto& emp : data){
//        auto entry = Gtk::make_managed<Entry>(emp);
//        list->append(*entry);
//    }
}

EmployeesTab::Entry::Entry(std::shared_ptr<Employeer> emp) : emp(emp) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    first_name = Gtk::make_managed<Gtk::Label>(emp->getFirstName());
    last_name = Gtk::make_managed<Gtk::Label>(emp->getLastName());
    patronymic = Gtk::make_managed<Gtk::Label>(emp->getPatronymic());
    address = Gtk::make_managed<Gtk::Label>(emp->getAddress());
    birth_date = Gtk::make_managed<Gtk::Label>(emp->getBirthDate());
    salary = Gtk::make_managed<Gtk::Label>( fmt::format("{}",emp->getSalary()));
    post = Gtk::make_managed<Gtk::Label>(emp->getPostAsString());

    box->add(*first_name);
    box->add(*last_name);
    box->add(*patronymic);
    box->add(*address);
    box->add(*birth_date);
    box->add(*salary);
    box->add(*post);

    this->add(*box);
}

std::shared_ptr<Employeer> EmployeesTab::Entry::get_emp() {
    return emp;
}

int EmployeesTab::Entry::get_id() {
    return emp->get_id();
}

bool EmployeesTab::scroll_down(){
//        first_id = last_id;
//        auto data = current_search->get_great_then(last_id,20);
//        if(data.empty()){
//            return false;
//        }
//        for(const auto& ing : data){
//            if(ing->get_id() > last_id){
//                last_id = ing->get_id();
//            }
//            auto entry = Gtk::make_managed<Entry>(ing);
//            getListBox()->add(*entry);
//        }
//
//        auto rows = getListBox()->get_children();
//        if(rows.size() > 40){
//
//            for(int i = 0; i < rows.size() - 40; i++){
//                fmt::print("removed\n");
//                getListBox()->remove(*rows[i]);
//            }
//        }
//        getListBox()->show_all();
//        scroll->get_vadjustment()->set_value(500);
        return true;
}
//
bool EmployeesTab::scroll_up(){
//        last_id = first_id;
//        auto data = current_search->get_less_then(first_id,20);
//        if(data.empty()){
//            return false;
//        }
//        for(const auto& ing : data){
//            if(ing->get_id() < first_id){
//                first_id = ing->get_id();
//            }
//            auto entry = Gtk::make_managed<Entry>(ing);
//            getListBox()->insert(*entry,0);
//        }
//
//        auto rows = getListBox()->get_children();
//        if(rows.size() > 40){
//            for(int i = rows.size() - 1; i >= 40; i--){
//                fmt::print("removed\n");
//                getListBox()->remove(*rows[i]);
//            }
//        }
//        getListBox()->show_all();
//
//        scroll->get_vadjustment()->set_value(100);
//
        return true;
}

// void EmployeesTab::scroll_event(Gtk::PositionType type){
//     if(type == Gtk::POS_BOTTOM){
//         scroll_down();
//     }else if(type == Gtk::POS_TOP){
//         scroll_up();
//     }
// }

EmployeesTab::DefaultSearch::DefaultSearch(EmployeerGateway* gateway) : gateway(gateway) {};

std::list<std::shared_ptr<Employeer>> EmployeesTab::DefaultSearch::get_great_then(int id, int count){
    return gateway->get_great_then_by_id(id,count);
}

std::list<std::shared_ptr<Employeer>> EmployeesTab::DefaultSearch::get_less_then(int id, int count){
    return gateway->get_less_then_by_id(id,count);
}

EmployeesTab::NameSearch::NameSearch(EmployeerGateway* gateway, std::string name) : gateway(gateway), name(name){}

std::list<std::shared_ptr<Employeer>> EmployeesTab::NameSearch::get_great_then(int id, int count){
    return gateway->get_great_then_by_name(name,id,count);
}

std::list<std::shared_ptr<Employeer>> EmployeesTab::NameSearch::get_less_then(int id, int count){
    return gateway->get_less_then_by_name(name,id,count);
}

IList* EmployeesTab::create_list(){

    auto list = Gtk::make_managed<EntityList<Employeer,Entry>>(&gateway);

    list->get_search_entry()->signal_activate().connect(sigc::bind<EntityList<Employeer,Entry>*>(&EmployeesTab::search,list));

    return list;
}

