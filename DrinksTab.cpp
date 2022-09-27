//
// Created by victor on 15.09.2022.
//

#include "DrinksTab.h"
#include "Form.h"
#include "gateways/Drinks/Drink.h"
#include "gtkmm/box.h"
#include "gateways/Ingredients/IngredientGateway.h"
#include "sigc++/functors/mem_fun.h"
#include <string>


DrinksTab::DrinksTab(TabManager *manager) : Tab(manager) {

    builder = Gtk::Builder::create_from_file("../drink_menu.glade");

    builder->get_widget("box",info_box);
    builder->get_widget("name_entry",name_entry);
    builder->get_widget("strength_entry",strength_entry);
    builder->get_widget("size_entry",size_entry);
    builder->get_widget("container_entry",container_entry);
    builder->get_widget("ing_list",ing_list);
    builder->get_widget("add_button",add_button);
    builder->get_widget("remove_button",remove_button);

    fill_list(getListBox());

    setup_menu(builder);

    auto save_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    save_button->signal_clicked().connect(sigc::mem_fun(this,&DrinksTab::save_current));
    info_box->add(*save_button);
    info_box->show_all();

    add_button->signal_clicked().connect(sigc::bind<Gtk::ListBox*,TabManager*>(
        &DrinksTab::add_ingredient,
        ing_list,
        get_tab_manager()
    ));

    remove_button->signal_clicked().connect(sigc::bind<Gtk::ListBox*>(
        &DrinksTab::remove_ingredient,
        ing_list
    ));

    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&DrinksTab::create));
    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&DrinksTab::remove_entry));

    getListBox()->signal_row_selected().connect(sigc::mem_fun(this,&DrinksTab::select));

    add_clumn_lable("название");
    add_clumn_lable("крепость");
    add_clumn_lable("размер");
    add_clumn_lable("ёмкость подачи");
}

int DrinksTab::Entry::get_id() {
    return drink->get_id();
}

DrinksTab::Entry::Entry(std::shared_ptr<Drink> drink) : drink(drink) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    name_label = Gtk::make_managed<Gtk::Label>(drink->getName());
    strength_label = Gtk::make_managed<Gtk::Label>(std::to_string(drink->getStrength()));
    size_label = Gtk::make_managed<Gtk::Label>(std::to_string(drink->getSize()));
    container_label = Gtk::make_managed<Gtk::Label>(drink->getContainer());

    box->add(*name_label);
    box->add(*strength_label);
    box->add(*size_label);
    box->add(*container_label);

    this->add(*box);
}

void DrinksTab::select(Gtk::ListBoxRow* row){
    auto entry = dynamic_cast<Entry*>(row);
    if(entry == nullptr){
        return;
    }

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    //replace box

    if(box->get_children().empty() || box->get_children()[0]->get_name() != "dring_menu"){
        if(!box->get_children().empty()){
            box->remove(*box->get_children()[0]);
        }
        box->add(*info_box);
    }

    auto drink = entry->get_drink();
    for(auto child : ing_list->get_children()){
        ing_list->remove(*child);
    }
    IngredientGateway ingredientGateway;
    for(auto ing : gateway.get_ingredients(drink)){
        std::shared_ptr<Ingredient> ingredient = ingredientGateway.get(std::get<0>(ing));
        auto row = Gtk::make_managed<Gtk::Box>();
        row->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        row->set_homogeneous(true);

        auto name_ing = Gtk::make_managed<Gtk::Label>(ingredient->get_name());
        auto count_ing = Gtk::make_managed<Gtk::Label>(std::to_string(std::get<1>(ing)));
        row->add(*name_ing);
        row->add(*count_ing);

        row->set_data("id", new int(ingredient->get_id()),[](void* data){delete (int*) data;});
        row->set_data("count", new int(std::get<1>(ing)), [](void* data){delete (int*) data;});

        ing_list->add(*row);
    }
    ing_list->show_all();

    name_entry->set_text(drink->getName());
    strength_entry->set_text(std::to_string(drink->getStrength()));
    size_entry->set_text(std::to_string(drink->getSize()));
    container_entry->set_text(drink->getContainer());

}

void DrinksTab::add_ingredient(Gtk::ListBox* list, TabManager* tab_manager) {
    int ing_id = tab_manager->select_dialog(TabName::INGREDIENTS);
    if(ing_id == -1){
        return;
    }

    Gtk::Dialog dialog;
    dialog.add_button("OK", Gtk::RESPONSE_OK);
    dialog.add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Box box;
    Gtk::Label label("количество");

    Gtk::Entry size_entry_dialog;
    size_entry_dialog.signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,&size_entry_dialog));

    box.add(size_entry_dialog);
    box.show_all();
    dynamic_cast<Gtk::Container*>(dialog.get_children()[0])->add(box);

    if(dialog.run() == Gtk::RESPONSE_OK){
        IngredientGateway ingredientGateway;
        std::shared_ptr<Ingredient> ingredient = ingredientGateway.get(ing_id);

        auto row = Gtk::make_managed<Gtk::Box>();
        row->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
        row->set_homogeneous(true);

        auto name_ing = Gtk::make_managed<Gtk::Label>(ingredient->get_name());
        auto count_ing = Gtk::make_managed<Gtk::Label>(size_entry_dialog.get_text());

        row->set_data("id", new int(ingredient->get_id()),[](void* data){delete (int*) data;});
        row->set_data("count", new int(std::stoi(size_entry_dialog.get_text())), [](void* data){delete (int*) data;});

        row->add(*name_ing);
        row->add(*count_ing);
        list->add(*row);
        list->show_all();
    }
}

void DrinksTab::save_current(){
    auto entry = dynamic_cast<Entry*>(getListBox()->get_selected_row());

    auto drink = entry->get_drink();

    if(name_entry->get_text().empty()){
        Gtk::MessageDialog message("не указано имя");
        message.run();
        return;
    }

    if(strength_entry->get_text().empty()){
        Gtk::MessageDialog message("не указана крепость");
        message.run();
        return;
    }

    if(size_entry->get_text().empty()){
        Gtk::MessageDialog message("не указан размер");
        message.run();
        return;
    }

    if(container_entry->get_text().empty()){
        Gtk::MessageDialog message("не указана ёмкость");
        message.run();
        return;
    }

//     std::cout << "old" << std::endl;

    std::vector<std::pair<int,int>> old_list;
    std::vector<std::pair<int,int>> new_list;

    for(auto& ing : gateway.get_ingredients(drink)){
//         std::cout << std::get<0>(ing) << " ";
        old_list.push_back(ing);
//         old_list.insert(ing);
    }
//     std::cout << std::endl;

//     std::cout << "new" << std::endl;

    for(auto row : ing_list->get_children()){

        auto box = dynamic_cast<Gtk::Container*>(row)->get_children()[0];

        int* id = static_cast<int*>(box->get_data("id"));
        int* count = static_cast<int*>(box->get_data("count"));
//         std::cout << *id << " ";
        new_list.push_back(std::make_pair(*id, *count));
//         old_list.insert(std::make_pair<int,int>(*id, *count));
    }
//     std::cout << std::endl;


    std::sort(old_list.begin(), old_list.end(),[](std::pair<int,int>& a, std::pair<int,int>& b){
        return a.first < b.first;
    });

    std::sort(new_list.begin(), new_list.end(),[](std::pair<int,int>& a, std::pair<int,int>& b){
        return a.first < b.first;
    });

    std::vector<std::pair<int,int>> created;
    std::vector<std::pair<int,int>> removed;

    std::set_difference(new_list.begin(), new_list.end(), old_list.begin(), old_list.end(),
                        std::inserter(created, created.begin()),
        [](std::pair<int,int>& a, std::pair<int,int>& b){
            return a.first < b.first;
        }

    );

    std::set_difference(old_list.begin(), old_list.end(), new_list.begin(), new_list.end(),
                        std::inserter(removed, removed.begin()),
        [](std::pair<int,int>& a,std::pair<int,int>& b){
            return a.first < b.first;
        }
    );


//     std::cout << "created" << std::endl;
    for(auto a : created){
//         std::cout << a.first << " ";
        drink->getRecipe().add_ingridient(a.first,a.second);
    }
//     std::cout << std::endl;

//     std::cout << "removed" << std::endl;
    for(auto a : removed){
//         std::cout <<  a.first << " ";
        drink->getRecipe().remove_ingridient(a.first);
    }
//     std::cout << std::endl;

    drink->setName(name_entry->get_text());
    drink->setStrength(std::stoi(strength_entry->get_text()));
    drink->setSize(std::stoi(size_entry->get_text()));
    drink->setContainer(container_entry->get_text());

    gateway.save(drink);

    entry->name_label->set_text(drink->getName());
    entry->strength_label->set_text(strength_entry->get_text());
    entry->size_label->set_text(size_entry->get_text());
    entry->container_label->set_text(drink->getContainer());

}

void DrinksTab::setup_menu(Glib::RefPtr<Gtk::Builder> builder){

    Gtk::Entry* strength_entry_menu;
    Gtk::Entry* size_entry_menu;

    builder->get_widget("strength_entry",strength_entry_menu);
    builder->get_widget("size_entry",size_entry_menu);

    strength_entry_menu->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,strength_entry_menu));

    size_entry_menu->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::number_only,size_entry_menu));
}

void DrinksTab::remove_ingredient(Gtk::ListBox* list) {
    auto row = list->get_selected_row();
    if(row == nullptr){
        return;
    }
    list->remove(*row);
}

void DrinksTab::create(){
    auto builder = Gtk::Builder::create_from_file("../drink_menu.glade");
    setup_menu(builder);

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Button* add_button_dialog;
    Gtk::Button* remove_button_dialog;
    Gtk::ListBox* ing_list_dialog;
    Gtk::Box* box;
    builder->get_widget("box", box);
    builder->get_widget("add_button",add_button_dialog);
    builder->get_widget("remove_button",remove_button_dialog);
    builder->get_widget("ing_list",ing_list_dialog);

    add_button_dialog->signal_clicked().connect(sigc::bind<Gtk::ListBox*,TabManager*>(
        &DrinksTab::add_ingredient,
        ing_list_dialog,
        get_tab_manager()
    ));

    remove_button_dialog->signal_clicked().connect(sigc::bind<Gtk::ListBox*>(
        &DrinksTab::remove_ingredient,
        ing_list_dialog
    ));

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this, dialog, builder](int response){
        if(response == Gtk::RESPONSE_OK){
            Gtk::Entry* name_entry_dialog;
            Gtk::Entry* strength_entry_dialog;
            Gtk::Entry* size_entry_dialog;
            Gtk::Entry* container_entry_dialog;
            Gtk::ListBox* ing_list_dialog;

            builder->get_widget("name_entry",name_entry_dialog);
            builder->get_widget("strength_entry",strength_entry_dialog);
            builder->get_widget("size_entry",size_entry_dialog);
            builder->get_widget("container_entry",container_entry_dialog);
            builder->get_widget("ing_list",ing_list_dialog);

            if(name_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указано имя");
                message.run();
                return;
            }

            if(strength_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указана крепость");
                message.run();
                return;
            }

            if(size_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указан размер");
                message.run();
                return;
            }

            if(container_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указана ёмкость");
                message.run();
                return;
            }

            std::vector<std::pair<int,int>> ings;

            for(auto row : ing_list_dialog->get_children()){
                auto box = dynamic_cast<Gtk::Container*>(row)->get_children()[0];
                int* id = static_cast<int*>(box->get_data("id"));
                int* count = static_cast<int*>(box->get_data("count"));

                ings.push_back(std::make_pair(*id, *count));
            }

            auto drink = gateway.create(
                name_entry_dialog->get_text(),
                std::stoi(strength_entry_dialog->get_text()),
                std::stoi(size_entry_dialog->get_text()),
                container_entry_dialog->get_text(),ings);

            auto entry = Gtk::make_managed<Entry>(drink);

            getListBox()->append(*entry);
            getListBox()->show_all();
        }
        dialog->close();
        delete dialog;
    });

    dialog->show_all();
}

void DrinksTab::remove_entry(){
    auto entry = dynamic_cast<Entry*>(getListBox()->get_selected_row());
    if(entry == nullptr){
        return;
    }

    gateway.remove(entry->get_drink());

    getListBox()->remove(*entry);
}

std::shared_ptr<Drink> DrinksTab::Entry::get_drink() {
    return drink;
}

// void DrinksTab::select_by_id(int entry_id) {
//
// }

void DrinksTab::fill_list(Gtk::ListBox *list) {
    for(const auto& drink : gateway.get_all()){
        auto entry = Gtk::make_managed<Entry>(drink);

        list->append(*entry);
    }
}

bool DrinksTab::scroll_down(){
        first_id = last_id;
        auto data = gateway.get_great_then_by_id(last_id,20);
        if(data.empty()){
            return false;
        }
        for(const auto& ing : data){
            if(ing->get_id() > last_id){
                last_id = ing->get_id();
            }
            auto entry = Gtk::make_managed<Entry>(ing);
            getListBox()->add(*entry);
        }

        auto rows = getListBox()->get_children();
        if(rows.size() > 40){

            for(int i = 0; i < rows.size() - 40; i++){
//                 fmt::print("removed\n");
                getListBox()->remove(*rows[i]);
            }
        }
        getListBox()->show_all();
        scroll->get_vadjustment()->set_value(500);
        return true;
}

bool DrinksTab::scroll_up(){
        last_id = first_id;
        auto data = gateway.get_less_then_by_id(first_id,20);
        if(data.empty()){
            return false;
        }
        for(const auto& ing : data){
            if(ing->get_id() < first_id){
                first_id = ing->get_id();
            }
            auto entry = Gtk::make_managed<Entry>(ing);
            getListBox()->insert(*entry,0);
        }

        auto rows = getListBox()->get_children();
        if(rows.size() > 40){
            for(int i = rows.size() - 1; i >= 40; i--){
                getListBox()->remove(*rows[i]);
            }
        }
        getListBox()->show_all();

        scroll->get_vadjustment()->set_value(100);

        return true;
}

