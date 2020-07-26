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

struct SessionModel
{
    std::string Id;
    std::string IdDocument;
    std::string Content;
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
    std::string _db_path;

public:
    Document(std::string db_path);
    ~Document();

    int createDocument(std::string title, std::string description, char *&id_to_insert);
    bool deleteDocument(char *&id);
    bool editDocument(char *&id, std::string title, std::string description);
    bool hasDocument(std::string id);
    int getDocument(std::string id);

    std::vector<std::unique_ptr<DocumentModel>> listAllDocuments();

    ///////////////////////////////////////////////////////////////////

    int addSession(char *idDocument, std::string content);
    bool deleteSession(char *&id);
    bool deleteAllSession(char *&idDocument);
    bool editSession(char *&id, std::string content);
    std::unique_ptr<SessionModel> getSession(std::string idDocument);
    int callbackDocument(int argc, char **argv, char **azColName);
};
