#include "document.h"

#include "globals.h"
#include <ctime>
#include <fstream>
#include <iostream>

#include <sqlite3.h>
#include <string.h>
#include <string>
#include <uuid/uuid.h>

using namespace std;

std::vector<DocumentModel> Document::listDocuments;

Document::Document(/* args */)
{
    // TODO: Modificar para config.
    db_path = "/home/mpulcinelli/Develop/personal/snap2text/static/data/db_snap2text.db";
}

Document::~Document()
{
}

int Document::staticCallbackDocument(void *param, int argc, char **argv, char **azColName)
{
    DocumentModel documentModel;

    for (int i = 0; i < argc; i++)
    {
        cout << azColName[i] << ": " << argv[i] << endl;
        if (strcmp(azColName[i], "Id") == 0)
            documentModel.Id = argv[i];
        else if (strcmp(azColName[i], "Title") == 0)
            documentModel.Title = argv[i];
        else if (strcmp(azColName[i], "Description") == 0)
            documentModel.Description = argv[i];
        else if (strcmp(azColName[i], "CreatedDate") == 0)
        {
            std::ifstream timeToStream(argv[i]);
            time_t t;
            timeToStream >> t;
            documentModel.CreatedDate = t;
        }
    }

    try
    {
        Document::listDocuments.push_back(documentModel);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    cout << endl;

    return 0;
}

int Document::callbackDocument(int argc, char **argv, char **azColName)
{
    DocumentModel documentModel;

    for (int i = 0; i < argc; i++)
    {
        cout << azColName[i] << ": " << argv[i] << endl;
        if (strcmp(azColName[i], "Id") == 0)
            documentModel.Id = argv[i];
        else if (strcmp(azColName[i], "Title") == 0)
            documentModel.Title = argv[i];
        else if (strcmp(azColName[i], "Description") == 0)
            documentModel.Description = argv[i];
        else if (strcmp(azColName[i], "CreatedDate") == 0)
        {
            std::ifstream timeToStream(argv[i]);
            time_t t;
            timeToStream >> t;
            documentModel.CreatedDate = t;
        }
    }

    try
    {
        Document::listDocuments.push_back(documentModel);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    cout << endl;

    return 0;
}

int Document::getDocument(std::string id)
{

    return 0;
}

bool Document::hasDocument(std::string id)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int exit = 0;

    clearList();

    exit = sqlite3_open(db_path.c_str(), &db);

    if (!exit)
    {

        std::string sql_raw = "SELECT * FROM tb_document where Id = '{@v0}';";

        findAndReplaceAll(sql_raw, "{@v0}", id);
        const char *sql_final = sql_raw.c_str();

        auto rc = sqlite3_exec(db, sql_final, staticCallbackDocument, 0, &zErrMsg);

        if (rc != SQLITE_OK)
        {
            cout << "Record NOT inserted!" << endl;
            cout << zErrMsg << endl;

            sqlite3_free(zErrMsg);
        }
        else
        {
            cout << zErrMsg << endl;
            cout << "Record inserted!" << endl;
        }
    }
    else
    {
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_close(db);

    return Document::listDocuments.size() > 0;
}

int Document::createDocument(std::string title, std::string description, char *&id_to_insert)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int exit = 0;

    std::string exec_path = db_path;

    exit = sqlite3_open(exec_path.c_str(), &db);

    if (!exit)
    {
        std::time_t result = std::time(nullptr);
        id_to_insert = new char[100];
        uuid_t id;

        uuid_generate_random(id);
        uuid_unparse(id, id_to_insert);

        std::string sql_raw = "INSERT INTO tb_document (Id, Title, Description, CreatedDate) VALUES ('{@v0}', '{@v1}','{@v2}', {@v3});";

        findAndReplaceAll(sql_raw, "{@v0}", id_to_insert);
        findAndReplaceAll(sql_raw, "{@v1}", title);
        findAndReplaceAll(sql_raw, "{@v2}", description);
        findAndReplaceAll(sql_raw, "{@v3}", std::to_string(result));

        const char *sql_final = sql_raw.c_str();

        auto rc = sqlite3_exec(db, sql_final, staticCallbackDocument, 0, &zErrMsg);

        if (rc != SQLITE_OK)
        {
            cout << "Record NOT inserted!" << endl;
            cout << zErrMsg << endl;

            sqlite3_free(zErrMsg);
        }
        else
        {
            cout << zErrMsg << endl;
            cout << "Record inserted!" << endl;
        }
    }
    else
    {
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_close(db);

    return 0;
}

bool Document::deleteDocument(char *&id)
{
    return false;
}
bool Document::editDocument(char *&id, std::string title, std::string description)
{
    return false;
}

int Document::addSession(char *idDocument, std::string content)
{

    sqlite3 *db;
    char *zErrMsg = 0;
    int exit = 0;

    std::string exec_path = db_path;

    exit = sqlite3_open(exec_path.c_str(), &db);

    if (!exit)
    {
        std::time_t result = std::time(nullptr);
        char *id_to_insert = new char[100];
        uuid_t id;

        uuid_generate_random(id);
        uuid_unparse(id, id_to_insert);

        std::string sql_raw = "INSERT INTO tb_session (Id, IdDocument, Content) VALUES ('{@v0}', '{@v1}','{@v2}');";

        findAndReplaceAll(sql_raw, "{@v0}", id_to_insert);
        findAndReplaceAll(sql_raw, "{@v1}", idDocument);
        findAndReplaceAll(sql_raw, "{@v2}", content);

        const char *sql_final = sql_raw.c_str();

        auto rc = sqlite3_exec(db, sql_final, staticCallbackDocument, 0, &zErrMsg);

        if (rc != SQLITE_OK)
        {
            cout << "Record NOT inserted!" << endl;
            cout << zErrMsg << endl;

            sqlite3_free(zErrMsg);
        }
        else
        {
            cout << zErrMsg << endl;
            cout << "Record inserted!" << endl;
        }
    }
    else
    {
        cout << "DB Open Error: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_close(db);

    return 0;
}

bool Document::deleteSession(char *&id)
{
    return false;
}
bool Document::deleteAllSession(char *&idDocument)
{
    return false;
}
bool Document::editSession(char *&id, std::string content)
{
    return false;
}

void Document::clearList()
{
    std::vector<DocumentModel>::iterator it = Document::listDocuments.begin();

    while (it != Document::listDocuments.end())
    {
        Document::listDocuments.erase(it++);
    }
}