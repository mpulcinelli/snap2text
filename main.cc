#include "areacapturewindow.h"
#include "drawingareawindow.h"
#include "globals.h"
#include "googletranslator.h"
#include "gridmodel.h"
#include "appintegrity.h"
#include "languagehelper.h"
#include "document.h"
#include <curl/curl.h>
#include <filesystem>
#include <fstream>
#include <gtk/gtk.h>
#include <gtk/gtkstyleprovider.h>
#include <gtkmm.h>
#include <gtkmm/enums.h>
#include <iostream>
#include <json/json.h>
#include <sstream>
#include <string.h>
#include <string>

using namespace std;

namespace fs = std::filesystem;

void load_app_config();
void list_tessfiledata();
void setup_components();
void load_language();

static Glib::RefPtr<Gtk::Application> app;

Json::Value app_config_params;

Glib::RefPtr<Gtk::Builder> builder;
Gtk::Window *main_window = nullptr;
Gtk::Window *menu_window = nullptr;
Gtk::ComboBoxText *cbo_lista_monitores;
Gtk::ComboBoxText *cbo_avail_lang_to_translate;
Gtk::ComboBoxText *cbo_languages_captura;
GridModel m_Columns;
Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

std::map<std::string, std::string> avail_lang_to_translate;
std::list<std::string> files_from_tesseract_data;
LanguageHelper *lhelper;

static void on_scan_finished(std::string texto)
{

    std::string texto_original = trim_copy(texto);

    if (main_window)
    {
        main_window->hide();

        Gtk::Box *box_results;
        builder->get_widget("box_results", box_results);
        if (box_results)
        {
            box_results->set_visible(true);

            Gtk::TextView *txt_text_original;

            builder->get_widget("txt_text_original", txt_text_original);
            if (txt_text_original)
            {
                txt_text_original->get_buffer()->set_text(texto_original);
            }
        }

        Gtk::Button *btn_action_traduzir;
        builder->get_widget("btn_action_traduzir", btn_action_traduzir);
        if (btn_action_traduzir)
        {
            btn_action_traduzir->set_visible(true);
        }
    }
}

static bool on_mnu_item_quit_clicked(GdkEventButton *button_event)
{
    if (app)
    {
        app->quit();
        return true;
    }

    return false;
}

static void on_tool_bar_item_add_to_compose_button_press_event()
{
    typedef Gtk::TreeModel::Children type_children;
    type_children children = m_refTreeModel->children();

    unsigned int i = 1;

    for (type_children::iterator iter = children.begin(); iter != children.end(); ++iter)
    {
        i++;
    }

    Gtk::ComboBoxText *cbo_select_to_compose;
    Glib::ustring txt_origem;
    builder->get_widget("cbo_select_to_compose", cbo_select_to_compose);
    if (cbo_select_to_compose)
    {
        Glib::ustring selected = cbo_select_to_compose->get_active_id();
        if (selected == "0")
        {
            Gtk::TextView *txt_text_original;
            Glib::ustring txt_origem;
            builder->get_widget("txt_text_original", txt_text_original);
            if (txt_text_original)
            {
                txt_origem = txt_text_original->get_buffer()->get_text();
                Gtk::TreeModel::Row row = *(m_refTreeModel->append());
                row[m_Columns.m_col_id] = i;
                row[m_Columns.m_col_text] = txt_origem;
            }
        }
        else if (selected == "1")
        {
            Gtk::TextView *txt_text_translated;
            Glib::ustring txt_translated;
            builder->get_widget("txt_text_translated", txt_text_translated);
            if (txt_text_translated)
            {
                txt_translated = txt_text_translated->get_buffer()->get_text();
                Gtk::TreeModel::Row row = *(m_refTreeModel->append());
                row[m_Columns.m_col_id] = i;
                row[m_Columns.m_col_text] = txt_translated;
            }
        }
    }
}

static void on_tool_bar_item_new_clicked()
{
    Gtk::TextView *txt_text_original;
    builder->get_widget("txt_text_original", txt_text_original);
    if (txt_text_original)
    {
        txt_text_original->get_buffer()->set_text("");
    }

    Gtk::TextView *txt_text_translated;
    builder->get_widget("txt_text_translated", txt_text_translated);
    if (txt_text_translated)
    {
        txt_text_translated->get_buffer()->set_text("");
    }
}

static void on_button_traduzir_clicked()
{
    Glib::ustring txt_origem;
    Glib::ustring txt_traduzido;
    Glib::ustring selected_language_from;
    Glib::ustring selected_language_to;

    Gtk::TextView *txt_text_original;
    builder->get_widget("txt_text_original", txt_text_original);
    if (txt_text_original)
    {
        txt_origem = txt_text_original->get_buffer()->get_text();
    }

    if (cbo_languages_captura)
    {
        selected_language_from = cbo_languages_captura->get_active_id();
    }
    else
    {
        return;
    }

    if (cbo_avail_lang_to_translate)
    {
        selected_language_to = cbo_avail_lang_to_translate->get_active_id();
    }
    else
    {
        return;
    }

    Gtk::TextView *txt_text_translated;
    builder->get_widget("txt_text_translated", txt_text_translated);

    GoogleTranslator gt(app_config_params);

    Json::Value root;

    root["q"] = txt_origem.c_str();
    root["target"] = selected_language_to.c_str();
    root["source"] = selected_language_from.c_str();

    Json::StreamWriterBuilder jsonbuilder;
    const std::string json_file = Json::writeString(jsonbuilder, root);
    std::cout << json_file << std::endl;

    auto response = gt.translateContent(json_file);

    if (txt_text_translated)
    {
        auto response_data = response["data"];
        auto trans_array = response_data["translations"];
        auto item_trans = trans_array[0];
        std::string translated = item_trans["translatedText"].asString();
        txt_text_translated->get_buffer()->set_text(translated);
    }
}

static void on_button_capture_clicked()
{
    int indx = -1;
    Glib::ustring selected_language;

    if (cbo_lista_monitores)
    {
        indx = cbo_lista_monitores->get_active_row_number();
    }
    else
    {
        return;
    }

    if (cbo_languages_captura)
    {
        selected_language = cbo_languages_captura->get_active_id();
    }
    else
    {
        return;
    }

    // TODO: Incluir mensagem de seleção de monitor inválida.
    if (indx < 0)
        return;

    //TODO: Incluir mensagem de seleção de linguagem inválida.
    if (selected_language.empty())
        return;

    if (builder)
    {
        if (!main_window)
        {
            builder->get_widget("main_window", main_window);
        }

        if (main_window)
        {
            main_window->remove();
            main_window->set_modal(true);

            DrawingAreaWindow *d = new DrawingAreaWindow(indx, selected_language);

            main_window->add(*d);

            d->signal_on_scan_finish().connect(sigc::ptr_fun(&on_scan_finished));
            d->show_all();

            Glib::RefPtr<Gdk::Screen> screen = menu_window->get_screen();

            main_window->fullscreen_on_monitor(screen, indx);
            main_window->show_all();
        }
    }
}

static void read_style()
{
    const Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
    const Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
    const Glib::RefPtr<Gtk::StyleContext> styleContext = Gtk::StyleContext::create();

    std::string exec_path = get_path_no_exe();

    try
    {
        css_provider->load_from_path(exec_path + "static/style.css");
        styleContext->add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(int argc, char *argv[])
{
    app = Gtk::Application::create(argc, argv, "br.com.marciopulcinelli.snap2text");
    AppIntegrity appInt;

    if (appInt.CheckConfigFilesIntegrity() != EAppIntegrityCheck::AppGoodToGo)
        return 1;

    read_style();
    load_app_config();
    list_tessfiledata();
    setup_components();
    load_language();

    char *id_to_insert;

    Document d;
    d.createDocument("A NOVA ORDEM", "A NOVA ORDEM MUNDIAL E OS TEMPLÁRIOS.", id_to_insert);

    if (menu_window)
        app->run(*menu_window);

    return 0;
}
void load_language()
{
    std::string l = app_config_params.get("app_language", "").asString();
    lhelper = new LanguageHelper(l);
    if (lhelper)
    {
        if (builder)
        {
            Gtk::MenuItem *mnu_file;
            builder->get_widget("mnu_file", mnu_file);
            if (mnu_file)
            {
                std::string mnu_file_label = lhelper->getItem("mnu_file_label");
                mnu_file->set_label(mnu_file_label);
            }

            Gtk::ImageMenuItem *mnu_item_quit;
            builder->get_widget("mnu_item_quit", mnu_item_quit);
            if (mnu_item_quit)
            {
                std::string mnu_item_quit_label = lhelper->getItem("mnu_item_quit_label");
                mnu_item_quit->set_label(mnu_item_quit_label);
            }

            Gtk::Label *lbl_display;
            builder->get_widget("lbl_display", lbl_display);
            if (lbl_display)
            {
                std::string lbl_display_label = lhelper->getItem("lbl_display_label");
                lbl_display->set_label(lbl_display_label);
            }

            Gtk::Label *lbl_capture_in;
            builder->get_widget("lbl_capture_in", lbl_capture_in);
            if (lbl_capture_in)
            {
                std::string lbl_capture_in_label = lhelper->getItem("lbl_capture_in_label");
                lbl_capture_in->set_label(lbl_capture_in_label);
            }

            Gtk::Label *lbl_translate_to;
            builder->get_widget("lbl_translate_to", lbl_translate_to);
            if (lbl_translate_to)
            {
                std::string lbl_translate_to_label = lhelper->getItem("lbl_translate_to_label");
                lbl_translate_to->set_label(lbl_translate_to_label);
            }

            Gtk::Button *btn_action_capturar;
            builder->get_widget("btn_action_capturar", btn_action_capturar);
            if (btn_action_capturar)
            {
                std::string btn_action_capturar_label = lhelper->getItem("btn_action_capturar_label");
                btn_action_capturar->set_label(btn_action_capturar_label);
            }

            Gtk::Button *btn_action_traduzir;
            builder->get_widget("btn_action_traduzir", btn_action_traduzir);
            if (btn_action_traduzir)
            {
                std::string btn_action_traduzir_label = lhelper->getItem("btn_action_traduzir_label");
                btn_action_traduzir->set_label(btn_action_traduzir_label);
            }
        }
    }
}
void load_app_config()
{

    std::string exec_path = get_path_no_exe();
    std::ifstream file_conf(exec_path + "app_config.json");

    if (file_conf.is_open())
    {
        Json::Reader json_reader;
        std::stringstream buffer;

        buffer << file_conf.rdbuf();
        file_conf.close();
        json_reader.parse(buffer, app_config_params);
    }
}

void list_tessfiledata()
{
    std::string exec_path = get_path_no_exe();
    std::string path = exec_path + "traineddata/";

    for (const auto &entry : fs::directory_iterator(path))
    {
        std::string file = entry.path().filename().c_str();

        std::size_t pos = file.find(".traineddata");

        std::string file_no_ext = file.substr(0, pos);

        files_from_tesseract_data.push_back(file_no_ext);
    }
}

void setup_components()
{
    GoogleTranslator gt(app_config_params);

    std::map<std::string, std::string> avail_lang_to_translate = gt.listAllAvailableLanguages();

    if (avail_lang_to_translate.empty())
        return;

    std::string exec_path = get_path_no_exe();

    /***************************************************************************************/
    // Carrega o recurso de interface.
    builder = Gtk::Builder::create_from_file(exec_path + "static/ui-window.glade");

    if (builder)
    {
        builder->get_widget("menu_window", menu_window);

        if (!menu_window)
            return;

        Gtk::Button *btn_action_capturar;
        builder->get_widget("btn_action_capturar", btn_action_capturar);
        if (btn_action_capturar)
        {
            // Usar pointer function para métodos estáticos.
            btn_action_capturar->signal_clicked().connect(sigc::ptr_fun(&on_button_capture_clicked));
        }

        Gtk::Button *btn_action_traduzir;
        builder->get_widget("btn_action_traduzir", btn_action_traduzir);
        if (btn_action_traduzir)
        {
            // Usar pointer function para métodos estáticos.
            btn_action_traduzir->signal_clicked().connect(sigc::ptr_fun(&on_button_traduzir_clicked));
        }

        Gtk::ImageMenuItem *mnu_item_quit;
        builder->get_widget("mnu_item_quit", mnu_item_quit);
        if (mnu_item_quit)
        {
            // Usar pointer function para métodos estáticos.
            mnu_item_quit->signal_button_press_event().connect(sigc::ptr_fun(&on_mnu_item_quit_clicked));
        }

        Gtk::ToolButton *tool_bar_item_add_to_compose;
        builder->get_widget("tool_bar_item_add_to_compose", tool_bar_item_add_to_compose);
        if (tool_bar_item_add_to_compose)
        {
            // Usar pointer function para métodos estáticos.
            tool_bar_item_add_to_compose->signal_clicked().connect(sigc::ptr_fun(&on_tool_bar_item_add_to_compose_button_press_event));
        }

        Gtk::ToolButton *tool_bar_item_new;
        builder->get_widget("tool_bar_item_new", tool_bar_item_new);
        if (tool_bar_item_new)
        {
            // Usar pointer function para métodos estáticos.
            tool_bar_item_new->signal_clicked().connect(sigc::ptr_fun(&on_tool_bar_item_new_clicked));
        }

        Gtk::TreeView *grd_captured_items;
        builder->get_widget("grd_captured_items", grd_captured_items);
        if (grd_captured_items)
        {
            m_refTreeModel = Gtk::ListStore::create(m_Columns);
            grd_captured_items->set_model(m_refTreeModel);

            grd_captured_items->append_column("Id", m_Columns.m_col_id);
            grd_captured_items->append_column_editable("Captured Text", m_Columns.m_col_text);

            for (guint i = 0; i < 2; i++)
            {
                grd_captured_items->get_column(i)->set_reorderable();
            }
        }

        // Recupera a quantidade de monitores do usuário.
        Glib::RefPtr<Gdk::Display> display = menu_window->get_display();
        const int num_monitors = display->get_n_monitors();

        builder->get_widget("cbo_monitors", cbo_lista_monitores);
        if (cbo_lista_monitores)
        {
            for (int i = 0; i < num_monitors; i++)
            {
                std::string descript = "";
                descript.append(display->get_monitor(i)->get_model());
                descript.append(" - ");
                descript.append(display->get_monitor(i)->get_manufacturer());
                cbo_lista_monitores->append(std::to_string(i).c_str(), descript);
            }
        }

        builder->get_widget("cbo_avail_lang_to_translate", cbo_avail_lang_to_translate);
        if (cbo_avail_lang_to_translate)
        {
            for (auto &i : avail_lang_to_translate)
            {
                cbo_avail_lang_to_translate->append(i.first, i.second);
            }
        }

        builder->get_widget("cbo_languages_captura", cbo_languages_captura);
        if (cbo_languages_captura)
        {
            for (auto &i : files_from_tesseract_data)
            {
                cbo_languages_captura->append(i, avail_lang_to_translate[i]);
            }
        }
    }
}