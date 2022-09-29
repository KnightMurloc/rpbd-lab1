//
// Created by victor on 05.09.2022.
//

#ifndef LAB1_PRODUCTTAB_H
#define LAB1_PRODUCTTAB_H

#include <gtkmm.h>
#include <memory>
#include "Tab.h"
#include "gateways/Product/ProductGateway.h"
#include "gtkmm/entry.h"

class ProductTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public IEntry {
        std::shared_ptr<Product> product;

    public:
        Gtk::Label* name_label;
        Gtk::Label* price_label;

        explicit Entry(std::shared_ptr<Product> product);

        std::shared_ptr<Product> get_product();

        int get_id() override;
    };

    ProductGateway gateway;
    std::unique_ptr<EntityList<Product,Entry>> list;

    Glib::RefPtr<Gtk::Builder> builder;

    Gtk::Box* info_box;

    Gtk::Label* ing_link;
    Gtk::Button* select_button_ing;
    Gtk::Button* find_button_ing;

    Gtk::Entry* price_entry;
    Gtk::Entry* delivery_terms_entry;
    Gtk::Entry* payment_terms_entry;

    Gtk::Label* provider_link;
    Gtk::Button* select_button_provider;
    Gtk::Button* find_button_provider;

    Gtk::Entry* name_entry;

    void select(Gtk::ListBoxRow* row);

    void find_ing();
    void find_provider();

    void save_current();

    void create();

    void remove_entry();

    static void select_ing(Gtk::Label* label, TabManager* manager);
    static void select_provider(Gtk::Label* label, TabManager* manager);

    static void setup_menu(Glib::RefPtr<Gtk::Builder> builder);

//     void scroll_event(Gtk::PositionType type);
protected:
    void fill_list(Gtk::ListBox* list) override;

    bool scroll_down() override;
    bool scroll_up() override;

    IList* create_list() override;
public:
    explicit ProductTab(TabManager* tab_manager);

    void cancel_current();

//     void select_by_id(int entry_id) override;

//    int select_dialog() override;
};


#endif //LAB1_PRODUCTTAB_H
