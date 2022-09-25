//
// Created by victor on 05.09.2022.
//

#include "ProductTab.h"
#include "Form.h"
#include "gateways/Product/Product.h"

#include <memory>
#include <string>
#include <utility>
#include <fmt/format.h>

ProductTab::ProductTab(TabManager* tab_manager) : Tab(tab_manager)  {
    builder = Gtk::Builder::create_from_file("../product_menu.glade");

    builder->get_widget("box",info_box);
    builder->get_widget("ing_link",ing_link);
    builder->get_widget("select_button_ing",select_button_ing);
    builder->get_widget("find_button_ing",find_button_ing);
    builder->get_widget("price_entry",price_entry);
    builder->get_widget("delivery_terms_entry",delivery_terms_entry);
    builder->get_widget("payment_terms_entry",payment_terms_entry);
    builder->get_widget("provider_link",provider_link);
    builder->get_widget("select_button_provider",select_button_provider);
    builder->get_widget("find_button_provider",find_button_provider);
    builder->get_widget("name_entry",name_entry);
    this->fill_list(getListBox());
    setup_menu(builder);

    auto save_button = Gtk::make_managed<Gtk::Button>(Gtk::StockID("gtk-save"));
    save_button->signal_clicked().connect(sigc::mem_fun(this,&ProductTab::save_current));
    info_box->add(*save_button);
    info_box->show_all();

    find_button_ing->signal_clicked().connect(sigc::mem_fun(this,&ProductTab::find_ing));
    find_button_provider->signal_clicked().connect(sigc::mem_fun(this,&ProductTab::find_provider));

    select_button_ing->signal_clicked().connect(sigc::bind<Gtk::Label*, TabManager*>(&ProductTab::select_ing,ing_link,get_tab_manager()));
    select_button_provider->signal_clicked().connect(sigc::bind<Gtk::Label*, TabManager*>(&ProductTab::select_provider,provider_link,get_tab_manager()));

    getAddButton()->signal_clicked().connect(sigc::mem_fun(this,&ProductTab::create));
    getRemoveButton()->signal_clicked().connect(sigc::mem_fun(this,&ProductTab::remove_entry));

    getListBox()->signal_row_selected().connect(sigc::mem_fun(this,&ProductTab::select));

    add_clumn_lable("название");
    add_clumn_lable("цена");
}

void ProductTab::save_current() {
    auto entry = dynamic_cast<Entry*>(getListBox()->get_selected_row());
    if(entry == nullptr){
        return;
    }

    if(price_entry->get_text().empty()){
        Gtk::MessageDialog message("не указана цена");
        message.run();
        return;
    }

    if(delivery_terms_entry->get_text().empty()){
        Gtk::MessageDialog message("не указаны условия поставки");
        message.run();
        return;
    }

    if(payment_terms_entry->get_text().empty()){
        Gtk::MessageDialog message("не указаны условия оплаты");
        message.run();
        return;
    }

    if(name_entry->get_text().empty()){
        Gtk::MessageDialog message("не указано имя");
        message.run();
        return;
    }

    auto product = entry->get_product();

    int* i_id_ptr = static_cast<int*>(ing_link->get_data("id"));
    int i_id = -1;
    if(i_id_ptr){
        i_id = *i_id_ptr;
    }

    int* p_id_ptr = static_cast<int*>(provider_link->get_data("id"));
    int p_id = -1;
    if(p_id_ptr){
        p_id = *p_id_ptr;
    }

    product->set_ingredient_id(i_id);
    product->set_price(std::stof(price_entry->get_text()));
    product->set_delivery_terms(delivery_terms_entry->get_text());
    product->set_payment_terms(payment_terms_entry->get_text());
    product->set_provider_id(p_id);
    product->set_name(name_entry->get_text());

    gateway.save(product);

    entry->name_label->set_text(product->get_name());
    entry->price_label->set_text(price_entry->get_text());
}

void ProductTab::create(){
    auto builder = Gtk::Builder::create_from_file("../product_menu.glade");
    setup_menu(builder);

    auto dialog = new Gtk::Dialog();
    dialog->add_button("OK", Gtk::RESPONSE_OK);
    dialog->add_button("отмена", Gtk::RESPONSE_CANCEL);

    Gtk::Label* ing_link_dialog;
    Gtk::Button* select_button_ing_dialog;
    Gtk::Button* find_button_ing_dialog;

    Gtk::Label* provider_link_dialog;
    Gtk::Button* select_button_provider_dialog;
    Gtk::Button* find_button_provider_dialog;

    builder->get_widget("ing_link",ing_link_dialog);
    builder->get_widget("select_button_ing",select_button_ing_dialog);
    builder->get_widget("find_button_ing",find_button_ing_dialog);
    builder->get_widget("provider_link",provider_link_dialog);
    builder->get_widget("select_button_provider",select_button_provider_dialog);
    builder->get_widget("find_button_provider",find_button_provider_dialog);

    find_button_ing_dialog->set_sensitive(false);
    find_button_provider_dialog->set_sensitive(false);

    select_button_ing_dialog->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&ProductTab::select_ing,ing_link_dialog, get_tab_manager()));

    select_button_provider_dialog->signal_clicked().connect(sigc::bind<Gtk::Label*,TabManager*>(&ProductTab::select_provider,provider_link_dialog, get_tab_manager()));

    Gtk::Box* box;
    builder->get_widget("box", box);

    dynamic_cast<Gtk::Container*>(dialog->get_children()[0])->add(*box);

    dialog->signal_response().connect([this,dialog, builder](int response){
        if(response == Gtk::RESPONSE_OK){
            Gtk::Entry* price_entry_dialog;
            Gtk::Entry* delivery_terms_entry_dialog;
            Gtk::Entry* payment_terms_entry_dialog;
            Gtk::Entry* name_entry_dialog;

            Gtk::Label* ing_link_dialog;
            Gtk::Label* provider_link_dialog;

            builder->get_widget("ing_link",ing_link_dialog);
            builder->get_widget("provider_link",provider_link_dialog);

            builder->get_widget("price_entry",price_entry_dialog);
            builder->get_widget("delivery_terms_entry",delivery_terms_entry_dialog);
            builder->get_widget("payment_terms_entry",payment_terms_entry_dialog);
            builder->get_widget("name_entry",name_entry_dialog);

            if(price_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указана цена");
                message.run();
                return;
            }

            if(delivery_terms_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указаны условия поставки");
                message.run();
                return;
            }

            if(payment_terms_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указаны условия оплаты");
                message.run();
                return;
            }

            if(name_entry_dialog->get_text().empty()){
                Gtk::MessageDialog message("не указано имя");
                message.run();
                return;
            }

            int* i_id_ptr = static_cast<int*>(ing_link_dialog->get_data("id"));
            int i_id = -1;
            if(i_id_ptr){
                i_id = *i_id_ptr;
            }

            int* p_id_ptr = static_cast<int*>(provider_link_dialog->get_data("id"));
            int p_id = -1;
            if(p_id_ptr){
                p_id = *p_id_ptr;
            }

            auto product = gateway.create(
              i_id,
              std::stof(price_entry_dialog->get_text()),
              delivery_terms_entry_dialog->get_text(),
              payment_terms_entry_dialog->get_text(),
              p_id,
              name_entry_dialog->get_text()
            );

            auto entry = Gtk::make_managed<Entry>(product);

            getListBox()->append(*entry);
            getListBox()->show_all();
        }

        dialog->close();
        delete dialog;
    });

    dialog->show_all();
}

void ProductTab::remove_entry(){
    auto entry = dynamic_cast<Entry*>(getListBox()->get_selected_row());
    if(entry == nullptr){
        return;
    }

    gateway.remove(entry->get_product());

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    box->remove(*box->get_children()[0]);

    getListBox()->remove(*entry);
}

void ProductTab::cancel_current() {

}

void ProductTab::fill_list(Gtk::ListBox* list) {
    for(const auto& product : gateway.get_all()){
        auto entry = Gtk::make_managed<Entry>(product);

        list->add(*entry);
    }
}

void ProductTab::select(Gtk::ListBoxRow* row) {
    auto entry = dynamic_cast<Entry*>(row);
    if(entry == nullptr){
        return;
    }

    Gtk::Box* box;
    Form::getInstance().getBuilder()->get_widget("info_box", box);

    //replace box
    if(box->get_children().empty() || box->get_children()[0]->get_name() != "product_menu"){
        if(!box->get_children().empty()){
            box->remove(*box->get_children()[0]);
        }
        box->add(*info_box);
    }

    try{
        ing_link->set_text(entry->get_product()->get_ingredient()->get_name());
        find_button_ing->set_sensitive(true);

        ing_link->set_data("id",new int(entry->get_product()->get_ingredient_id()),[](void* data){delete (int*) data;});
    }catch(GatewayException&){
        ing_link->set_text("none");
        find_button_ing->set_sensitive(false);
        ing_link->set_data("id",nullptr,free);
    }

    price_entry->set_text(fmt::format("{}",entry->get_product()->get_price()));
    delivery_terms_entry->set_text(entry->get_product()->get_delivery_terms());
    payment_terms_entry->set_text(entry->get_product()->get_payment_terms());

    try{
        provider_link->set_text(entry->get_product()->get_provider()->get_name());
        find_button_provider->set_sensitive(true);
        provider_link->set_data("id", new int(entry->get_product()->get_provider_id()),[](void* data){delete (int*) data;});
    }catch(GatewayException&){
        provider_link->set_text("none");
        find_button_provider->set_sensitive(false);
        provider_link->set_data("id",nullptr,free);
    }

    name_entry->set_text(entry->get_product()->get_name());
}

void ProductTab::find_ing() {
    int* id = static_cast<int*>(ing_link->get_data("id"));
    if(id)
        get_tab_manager()->select_on_tab(TabName::INGREDIENTS,*id);
}

void ProductTab::find_provider() {
    int* id = static_cast<int*>(provider_link->get_data("id"));
    if(id)
        get_tab_manager()->select_on_tab(TabName::PROVIDER,*id);
}

void ProductTab::select_ing(Gtk::Label* label, TabManager* manager) {
    int id = manager->select_dialog(TabName::INGREDIENTS);
    if(id != -1){
        try {
            IngredientGateway ingredientGateway;
            label->set_text(ingredientGateway.get(id)->get_name());
        }catch(GatewayException&e){
            return;
        }
        label->set_data("id", new int(id), [](void* data){delete (int*) data;});
    }
}

void ProductTab::select_provider(Gtk::Label* label, TabManager* manager) {
    int id = manager->select_dialog(TabName::PROVIDER);
    if(id != -1){
        try {
            ProviderGateway providerGateway;
            label->set_text(providerGateway.get(id)->get_name());
        }catch(GatewayException&e){
            return;
        }
        label->set_data("id", new int(id), [](void* data){delete (int*) data;});
    }
}

void ProductTab::setup_menu(Glib::RefPtr<Gtk::Builder> builder){
    Gtk::Entry* price_entry_menu;
    builder->get_widget("price_entry",price_entry_menu);

    price_entry_menu->signal_insert_text().connect(sigc::bind<Gtk::Entry*>(&Form::float_only, price_entry_menu));
}


ProductTab::Entry::Entry(std::shared_ptr<Product> product) : product(std::move(product)) {
    auto box = Gtk::make_managed<Gtk::Box>();
    box->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box->set_homogeneous(true);

    name_label = Gtk::make_managed<Gtk::Label>(this->product->get_name());
    price_label = Gtk::make_managed<Gtk::Label>(fmt::format("{}",this->product->get_price()));
//     provider_label = Gtk::make_managed<Gtk::Label>("none");

//     try{
//         ingredient_label->set_text(this->product.get_ingredient().get_name());
//     }catch(GatewayException&){}

    box->add(*name_label);
    box->add(*price_label);

    this->add(*box);
}

std::shared_ptr<Product> ProductTab::Entry::get_product() {
    return product;
}

int ProductTab::Entry::get_id() {
    return product->get_id();
}
