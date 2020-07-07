#include "document.h"
#include "globals.h"
#include <string>
#include <sqlite3.h>
#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;

Document::Document(/* args */)
{
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

int Document::createDocument(std::string title, std::string description)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int exit = 0;
    std::string exec_path = get_path_no_exe();
    exec_path = exec_path + "static/data/db_snap2text.db";

    exit = sqlite3_open(exec_path.c_str(), &db);

    if (!exit)
    {
        std::time_t result = std::time(nullptr);

        cout << "Opened Database Successfully!" << endl;

        std::string sql_raw = "INSERT INTO tb_document (Title, Description, CreatedDate) VALUES ('{@v1}','{@v2}', {@v3});";
        //title.c_str(), description.c_str(), result

        findAndReplaceAll(sql_raw, "{@v1}", title);
        findAndReplaceAll(sql_raw, "{@v2}", description);
        findAndReplaceAll(sql_raw, "{@v3}", std::to_string(result));

        auto rc = sqlite3_exec(db, sql_raw.c_str(), c_callback, 0, &zErrMsg);
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
