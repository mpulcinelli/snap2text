#include "document.h"
#include "globals.h"
#include <string>
#include <sqlite3.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <uuid/uuid.h>

using namespace std;

Document::Document(/* args */)
{
    db_path = "/home/mpulcinelli/Develop/personal/snap2text/static/data/db_snap2text.db";
}

Document::~Document()
{
}

static int c_callback(void *param, int argc, char **argv, char **azColName)
{
    Document *cust = reinterpret_cast<Document *>(param);
    return cust->Callback(argc, argv, azColName);
}

int Document::Callback(int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        cout << azColName[i] << ": " << argv[i] << endl;
    }

    cout << endl;

    return 0;
}

int Document::createDocument(std::string title, std::string description, char *&id_to_insert)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int exit = 0;
    //std::string exec_path = get_path_no_exe();
    //exec_path = exec_path + "static/data/db_snap2text.db";
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

        auto rc = sqlite3_exec(db, sql_final, c_callback, 0, &zErrMsg);

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

bool Document::deleteDocument(int id)
{
    return false;
}
bool Document::editDocument(int id, std::string title, std::string description)
{
    return false;
}

int Document::addSession(int idDocument, std::string content)
{
    return 0;
}

bool Document::deleteSession(int id)
{
    return false;
}
bool Document::deleteAllSession(int idDocument)
{
    return false;
}
bool Document::editSession(int id, std::string content)
{
    return false;
}
