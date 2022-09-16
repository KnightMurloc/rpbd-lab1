//
// Created by victor on 16.09.2022.
//

#ifndef LAB1_PROVIDERTAB_H
#define LAB1_PROVIDERTAB_H

#include "Tab.h"
#include "gateways/Provider/Provider.h"
#include "gateways/Provider/ProviderGateway.h"

class ProviderTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public Tab::IEntry {
        Provider provider;
    public:
        Gtk::Label* name_label;
        Gtk::Label* post_address_label;
        Gtk::Label* phone_number_label;
        Gtk::Label* fax_label;
        Gtk::Label* email_label;

        explicit Entry(Provider provider);

        const Provider &get_provider() const;

        int get_id() override;
    };

    ProviderGateway gateway;
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

protected:
    void fill_list(Gtk::ListBox* list) override;

public:
    ProviderTab(TabManager* manager);

//     void select_by_id(int entry_id) override;
};


#endif //LAB1_PROVIDERTAB_H
