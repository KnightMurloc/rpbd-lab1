//
// Created by victor on 16.09.2022.
//

#ifndef LAB1_PROVIDERTAB_H
#define LAB1_PROVIDERTAB_H

#include "Tab.h"
#include "gateways/Provider/Provider.h"
#include "gateways/Provider/ProviderGateway.h"
#include "gtkmm/builder.h"

class ProviderTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public IEntry {
        std::shared_ptr<Provider> provider;
    public:
        Gtk::Label* name_label;
        Gtk::Label* post_address_label;
        Gtk::Label* phone_number_label;
        Gtk::Label* fax_label;
        Gtk::Label* email_label;

        explicit Entry(std::shared_ptr<Provider> provider);

        std::shared_ptr<Provider> get_provider();

        int get_id() override;
    };

    ProviderGateway gateway;

    EntityList<Provider,Entry>* list;

    Glib::RefPtr<Gtk::Builder> builder;

    Gtk::Box* info_box;
    Gtk::Entry* name_entry;
    Gtk::Entry* address_entry;
    Gtk::Entry* entry_number;
    Gtk::Entry* fax_entry;
    Gtk::Entry* email_entry;
    Gtk::Label* detail_link;
    Gtk::Button* select_button;
    Gtk::Button* find_button;

    void select(Gtk::ListBoxRow* row);

    void find_detail();

    static void select_deltail(Gtk::Label* label, TabManager* manager);

    void save_current();

    void create();

//     void setup_menu(Glib::RefPtr<Gtk::Builder> builder);

    void remove_entry();

protected:
    IList* create_list() override;
public:
    ProviderTab(TabManager* manager);
};


#endif //LAB1_PROVIDERTAB_H
