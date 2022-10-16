//
// Created by victor on 27.09.2022.
//

#ifndef LAB1_ENTITYLIST_H
#define LAB1_ENTITYLIST_H

#include <gtkmm.h>
#include <memory>
#include "fmt/core.h"
#include "gateways/Gateway.h"

class IEntry {
public:
    virtual int get_id() = 0;
};

class IList{
public:
    virtual IEntry* get_selected() = 0;
    virtual void fill_list() = 0;
    virtual void select_row(Gtk::ListBoxRow* widget) = 0;
    virtual bool scroll_up() = 0;
    virtual bool scroll_down() = 0;
    virtual std::vector<Gtk::Widget*> get_children() = 0;
    virtual void remove_row(Gtk::Widget* widget) = 0;

    virtual ~IList(){
        fmt::print("remove list\n");
    }
};

template<typename T>
class ISearch{
public:
    virtual std::list<std::shared_ptr<T>> get_great_then(int id, int count) = 0;
    virtual std::list<std::shared_ptr<T>> get_less_then(int id, int count) = 0;
};

template<class T, class E>
class EntityList : public Gtk::Box, public IList{
private:

    class DefaultSearch : public ISearch<T> {
    private:
//         IGateway<T>* gateway;
    public:

//         DefaultSearch(IGateway<T>* gateway);

        std::list<std::shared_ptr<T>> get_great_then(int id, int count);
        std::list<std::shared_ptr<T>> get_less_then(int id, int count);
    };

    Gtk::Box* header;
    Gtk::ListBox* list;
    Gtk::SearchEntry* search_entry;
    Gtk::Box* box;
    Gtk::ScrolledWindow* scroll;
//     IGateway<T>* gateway;

    std::function<void(E*)> select_callback = nullptr;

    int first_id;
    int last_id;

    std::unique_ptr<ISearch<T>> current_search;

    void scroll_event(Gtk::PositionType type);

    void select(Gtk::ListBoxRow* row);
public:
    explicit EntityList();

    void fill_list() override;

    bool scroll_down() override;
    bool scroll_up() override;

    Gtk::ListBox* get_list_box();

    void add_entity(E* entity);
    void remove_entity(E* entity);

    void set_search(std::unique_ptr<ISearch<T>> search);

    void reset_search();

    std::string get_search_text();

    IEntry* get_selected() override;

    IGateway<T>* get_gateway();

    Gtk::SearchEntry* get_search_entry();

    std::vector<Gtk::Widget*> get_children() override;

    void select_row(Gtk::ListBoxRow* widget) override;

    void remove_row(Gtk::Widget* widget) override;

    void add_column_title(std::string title);

//     ~EntityList(){
//         fmt::print("remove list\n");
//     }
};

#include "EntityList.inc"

#endif //LAB1_ENTITYLIST_H
