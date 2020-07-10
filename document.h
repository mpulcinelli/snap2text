#include <string>
#include <ctime>
#include <vector>

struct DocumentModel
{
    std::string Id;
    std::string Title;
    std::string Description;
    std::time_t CreatedDate;
};

class Document
{
private:
    /* data */
    std::string db_path;
    void clearList();
    static int staticCallbackDocument(void *param, int argc, char **argv, char **azColName);

    static std::vector<DocumentModel> listDocuments;

public:
    Document(/* args */);
    ~Document();

    int createDocument(std::string title, std::string description, char *&id_to_insert);
    bool deleteDocument(char *&id);
    bool editDocument(char *&id, std::string title, std::string description);
    bool hasDocument(std::string id);
    ///////////////////////////////////////////////////////////////////

    int addSession(char *idDocument, std::string content);
    bool deleteSession(char *&id);
    bool deleteAllSession(char *&idDocument);
    bool editSession(char *&id, std::string content);
    int callbackDocument(int argc, char **argv, char **azColName);
};
