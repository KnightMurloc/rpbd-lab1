//
// Created by victor on 02.10.2022.
//

#ifndef LAB1_DRINKORDERSTAB_H
#define LAB1_DRINKORDERSTAB_H

#include "Tab.h"
#include "gateways/DrinkOrders/DrinkOrderGateway.h"
#include "EntityList.h"
#include "gateways/entity.h"

class DrinkOrdersTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public IEntry {
        std::shared_ptr<DrinkOrder> order;

    public:
        Gtk::Label* drink_label;
        Gtk::Label* waiter_label;
        Gtk::Label* table_label;

        explicit Entry(std::shared_ptr<DrinkOrder> order);

        std::shared_ptr<DrinkOrder> get_order();

        int get_id() override;
    };
    Glib::RefPtr<Gtk::Builder> builder;

//     DrinkOrderGateway gateway;

    EntityList<DrinkOrder,Entry>* list;

    Gtk::Box* info_box;

    Gtk::Label* drink_link;
    Gtk::Button* drink_find;
    Gtk::Button* drink_select;

    Gtk::Label* waiter_link;
    Gtk::Button* waiter_find;
    Gtk::Button* waiter_select;

    Gtk::Entry* table_entry;

    static void setup_menu(Glib::RefPtr<Gtk::Builder> builder);

    void select(Gtk::ListBoxRow* row);

    static void select_drink(Gtk::Label* label, TabManager* manager);
    static void select_waiter(Gtk::Label* label, TabManager* manager);

    void find_drink();
    void find_waiter();

    void create();
    void remove();

    void save_current();

    void remove_drink_callback(std::shared_ptr<IEntity> entity);
    void remove_waiter_callback(std::shared_ptr<IEntity> entity);
protected:
    IList* create_list() override;
public:
    explicit DrinkOrdersTab(TabManager* tab_manager);
};


#endif //LAB1_DRINKORDERSTAB_H
