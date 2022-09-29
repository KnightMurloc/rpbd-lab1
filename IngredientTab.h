//
// Created by victor on 12.09.2022.
//

#ifndef LAB1_INGREDIENTTAB_H
#define LAB1_INGREDIENTTAB_H

#include "Tab.h"
#include "gateways/Ingredients/Ingredient.h"
#include "gateways/Ingredients/IngredientGateway.h"
#include <functional>

class IngredientTab : public Tab {
private:
    class Entry : public Gtk::ListBoxRow, public IEntry {
        std::shared_ptr<Ingredient> ingredient;
    public:
        Gtk::Label* name_label;
        Gtk::Label* unit_label;

        Entry(std::shared_ptr<Ingredient> ingredient);

        std::shared_ptr<Ingredient> get_ingredient();

        int get_id() override;
    };

    class DefaultSearch : public ISearch<Ingredient> {
    private:
        IngredientGateway* gateway;
    public:
        DefaultSearch(IngredientGateway* gateway);
        std::list<std::shared_ptr<Ingredient>> get_great_then(int id, int count);
        std::list<std::shared_ptr<Ingredient>> get_less_then(int id, int count);
    };

    class NameSearch : public ISearch<Ingredient> {
    private:
        IngredientGateway* gateway;
        std::string name;
    public:
        NameSearch(IngredientGateway* gateway, std::string name);
        std::list<std::shared_ptr<Ingredient>> get_great_then(int id, int count);
        std::list<std::shared_ptr<Ingredient>> get_less_then(int id, int count);
    };

    IngredientGateway gateway;
    Glib::RefPtr<Gtk::Builder> builder;

    Gtk::Box* info_box;
    Gtk::Entry* name_entry;
    Gtk::ComboBoxText* unit_combo;

//    std::unique_ptr<ISearch<Ingredient>> current_search;
//     std::unique_ptr<EntityList<Ingredient,Entry>> list;
    EntityList<Ingredient,Entry>* list;

//     std::function<std::list<std::shared_ptr<Ingredient>(int,int)>> get_data;
//     std::function<void(int,int)> get_data;
//     std::function<std::list<std::shared_ptr<Ingredient>>(int,int)> get_data;

    void remove_entry();

    void create();

    void select(Gtk::ListBoxRow* row);

    void save();

//     void scroll_event(Gtk::PositionType type);

    static void search(EntityList<Ingredient,Entry>* list);

    void search_stop();
protected:
//     void fill_list(Gtk::ListBox* list) override;

//     bool scroll_down() override;
//     bool scroll_up() override;

    IList* create_list() override;
public:

    explicit IngredientTab(TabManager* tab_manager);

//     void select_by_id(int entry_id) override;

//    int select_dialog() override;
};


#endif //LAB1_INGREDIENTTAB_H
