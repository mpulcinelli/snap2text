#include <ctime>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>
#include <string>
#include <vector>

struct DocumentModel
{
    std::string Id;
    std::string Title;
    std::string Description;
    std::time_t CreatedDate;
};
class DocumentCombobox : public Gtk::TreeModel::ColumnRecord
{
private:
    /* data */
public:
    DocumentCombobox()
    {
        add(m_col_id);
        add(m_col_name);
        add(m_col_extra);
    }

    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<Glib::ustring> m_col_extra;
};

class Document
{
private:
    /* data */
    std::string db_path;
    void clearList();
    static int staticCallbackDocument(void *param, int argc, char **argv, char **azColName);

    std::vector<std::unique_ptr<DocumentModel>> listDocuments;

public:
    Document(/* args */);
    ~Document();

    int createDocument(std::string title, std::string description, char *&id_to_insert);
    bool deleteDocument(char *&id);
    bool editDocument(char *&id, std::string title, std::string description);
    bool hasDocument(std::string id);
    int getDocument(std::string id);

    const std::vector<std::unique_ptr<DocumentModel>> &listAllDocuments() const;

    ///////////////////////////////////////////////////////////////////

    int addSession(char *idDocument, std::string content);
    bool deleteSession(char *&id);
    bool deleteAllSession(char *&idDocument);
    bool editSession(char *&id, std::string content);
    int callbackDocument(int argc, char **argv, char **azColName);
};
