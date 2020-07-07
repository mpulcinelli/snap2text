#include <string>

class Document
{
private:
    /* data */

public:
    Document(/* args */);
    ~Document();

    int createDocument(std::string title, std::string description);
    bool deleteDocument(int id);
    bool editDocument(int id, std::string title, std::string description);

    ///////////////////////////////////////////////////////////////////

    int addSession(int idDocument, std::string content);
    bool deleteSession(int id);
    bool deleteAllSession(int idDocument);
    bool editSession(int id, std::string content);
    int Callback(int argc, char **argv, char **azColName);
};
