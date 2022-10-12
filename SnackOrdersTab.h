//
// Created by victor on 30.09.2022.
//

#ifndef LAB1_SNACKORDERSTAB_H
#define LAB1_SNACKORDERSTAB_H

#include "Tab.h"
#include "gateways/SnackOrders/SnackOrdergateway.h"
#include "EntityList.h"
#include "gateways/entity.h"

class SnackOrdersTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public IEntry {
        std::shared_ptr<SnackOrder> order;

    public:
        Gtk::Label* drink_label;
        Gtk::Label* waiter_label;
        Gtk::Label* table_label;

        explicit Entry(std::shared_ptr<SnackOrder> order);

        std::shared_ptr<SnackOrder> get_order();

        int get_id() override;
    };
    Glib::RefPtr<Gtk::Builder> builder;

//     SnackOrdergateway gateway;

    EntityList<SnackOrder,Entry>* list;

    Gtk::Box* info_box;

    Gtk::Label* snack_link;
    Gtk::Button* snack_find;
    Gtk::Button* snack_select;

    Gtk::Label* waiter_link;
    Gtk::Button* waiter_find;
    Gtk::Button* waiter_select;

    Gtk::Entry* table_entry;

    static void setup_menu(Glib::RefPtr<Gtk::Builder> builder);

    void select(Gtk::ListBoxRow* row);

    static void select_snack(Gtk::Label* label, TabManager* manager);
    static void select_waiter(Gtk::Label* label, TabManager* manager);

    void find_snack();
    void find_waiter();

    void create();
    void remove();

    void save_current();

    void remove_snack_callback(std::shared_ptr<IEntity> entity);
    void remove_waiter_callback(std::shared_ptr<IEntity> entity);
protected:
    IList* create_list() override;
public:
    explicit SnackOrdersTab(TabManager* tab_manager);
};


#endif //LAB1_SNACKORDERSTAB_H
