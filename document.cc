#include "document.h"

#include "globals.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <sqlite3.h>
#include <string.h>
#include <string>
#include <uuid/uuid.h>
#include <vector>

using namespace std;

Document::Document(std::string db_path)
{
    // TODO: Modificar para config.
    _db_path = db_path;
    // "/home/mpulcinelli/Develop/personal/snap2text/static/data/db_snap2text.db";
}

Document::~Document()
{
}

int Document::getDocument(std::string id)
{

    return 0;
}

bool Document::hasDocument(std::string id)
{
    return 0;
}

std::vector<std::unique_ptr<DocumentModel>> Document::listAllDocuments()
{
    sqlite3 *db = nullptr;
    sqlite3_stmt *statement = nullptr;

    std::vector<std::unique_ptr<DocumentModel>> listDocumentsPtr;

    int retval = sqlite3_open(_db_path.c_str(), &db);

    if (retval != SQLITE_OK)
    {
        cerr << "open: " << sqlite3_errstr(retval) << '\n';
        return listDocumentsPtr;
    }

    string zSql("SELECT * FROM tb_document;");

    sqlite3_prepare_v2(db, zSql.c_str(), -1, &statement, nullptr);

    const int num_cols = sqlite3_column_count(statement);
    int i = 0;

    while (sqlite3_step(statement) != SQLITE_DONE)
    {
        unique_ptr<DocumentModel> documentModel = make_unique<DocumentModel>();

        for (i = 0; i < num_cols; i++)
        {
            auto col_name = sqlite3_column_name(statement, i);
            auto col_name_ascii = converttoASCII(col_name);
            cout << col_name << " : " << col_name_ascii << endl;

            switch (col_name_ascii)
            {
            case _ID:
                documentModel->Id = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, i)));
                break;
            case _TITLE:
                documentModel->Title = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, i)));
                break;
            case _DESCRIPTION:
                documentModel->Description = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, i)));
                break;
            case _CREATEDDATE:
                documentModel->CreatedDate = sqlite3_column_int(statement, i);
                break;
            default:
                break;
            }
        }

        listDocumentsPtr.push_back(std::move(documentModel));
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);

    return listDocumentsPtr;
}

int Document::createDocument(std::string title, std::string description, char *&id_to_insert)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int exit = 0;

    exit = sqlite3_open(_db_path.c_str(), &db);

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

        auto rc = sqlite3_exec(db, sql_final, NULL, 0, &zErrMsg);

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

std::unique_ptr<SessionModel> Document::getSession(std::string idDocument)
{
    sqlite3 *db = nullptr;
    sqlite3_stmt *statement = nullptr;

    int retval = sqlite3_open(_db_path.c_str(), &db);

    if (retval != SQLITE_OK)
    {
        cerr << "open: " << sqlite3_errstr(retval) << '\n';
        return nullptr;
    }

    string sql_raw("SELECT Id, Content FROM tb_session where IdDocument = '{@v0}'");

    findAndReplaceAll(sql_raw, "{@v0}", idDocument);

    sqlite3_prepare_v2(db, sql_raw.c_str(), -1, &statement, nullptr);

    const int num_cols = sqlite3_column_count(statement);
    int i = 0;

    unique_ptr<SessionModel> sessionModel = make_unique<SessionModel>();

    while (sqlite3_step(statement) != SQLITE_DONE)
    {
        for (i = 0; i < num_cols; i++)
        {
            auto col_name = sqlite3_column_name(statement, i);
            auto col_name_ascii = converttoASCII(col_name);
            cout << col_name << " : " << col_name_ascii << endl;

            switch (col_name_ascii)
            {
            case _ID:
                sessionModel->Id = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, i)));
                break;
            case _TITLE:
                sessionModel->IdDocument = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, i)));
                break;
            case _DESCRIPTION:
                sessionModel->Content = std::string(reinterpret_cast<const char *>(sqlite3_column_text(statement, i)));
                break;
            default:
                break;
            }
        }
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);

    return std::move(sessionModel);
}

int Document::addSession(char *idDocument, std::string content)
{

    sqlite3 *db;
    char *zErrMsg = 0;
    int exit = 0;

    exit = sqlite3_open(_db_path.c_str(), &db);

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

        auto rc = sqlite3_exec(db, sql_final, NULL, 0, &zErrMsg);

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
