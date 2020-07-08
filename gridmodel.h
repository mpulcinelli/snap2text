//Tree model columns:
#include <gtkmm.h>

class GridModel : public Gtk::TreeModel::ColumnRecord
{
public:
    GridModel()
    {
        add(m_col_id);
        add(m_col_text);
    }

    Gtk::TreeModelColumn<unsigned int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_text;
};