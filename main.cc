#include "areacapturewindow.h"
#include "curlreader.h"
#include "drawingareawindow.h"
#include "globals.h"
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
void list_tessdata();

Json::Value app_config_params;

Glib::RefPtr<Gtk::Builder> builder;
Gtk::Window *main_window = nullptr;
Gtk::Window *menu_window = nullptr;
Gtk::ComboBoxText *cbo_lista_monitores;
Gtk::ComboBoxText *cbo_avail_lang_to_translate;
Gtk::ComboBoxText *cbo_languages_captura;

std::map<std::string, std::string> avail_lang_to_translate;
std::list<std::string> files_from_tesseract_data;

static void on_scan_finished(char *texto)
{

    std::string texto_original = trim_copy(std::string(texto));

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

static void on_button_traduzir_clicked()
{
    Glib::ustring txt_origem;
    Glib::ustring txt_traduzido;

    Gtk::TextView *txt_text_original;
    builder->get_widget("txt_text_original", txt_text_original);
    if (txt_text_original)
    {
        txt_origem = txt_text_original->get_buffer()->get_text();
    }

    Gtk::TextView *txt_text_translated;
    builder->get_widget("txt_text_translated", txt_text_translated);

    CurlReader read(app_config_params);

    Json::Value root;

    root["q"] = txt_origem.c_str();
    root["target"] = "pt";
    root["source"] = "en";

    Json::StreamWriterBuilder jsonbuilder;
    const std::string json_file = Json::writeString(jsonbuilder, root);
    std::cout << json_file << std::endl;

    auto response = read.translate_content(json_file);

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

    if (cbo_lista_monitores)
    {
        indx = cbo_lista_monitores->get_active_row_number();
    }

    // TODO: Incluir mensagem de seleção de monitor inválida.
    if (indx < 0)
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

            DrawingAreaWindow *d = new DrawingAreaWindow(indx);

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

    css_provider->load_from_path("./static/style.css");
    styleContext->add_provider_for_screen(screen, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

int main(int argc, char *argv[])
{
    static Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "br.com.marciopulcinelli.snap2text");

    read_style();
    load_app_config();
    list_tessdata();

    CurlReader read(app_config_params);
    std::map<std::string, std::string> avail_lang_to_translate = read.read_available_languages();

    /***************************************************************************************/

    builder = Gtk::Builder::create_from_file("./static/ui-window.glade");

    if (builder)
    {
        builder->get_widget("menu_window", menu_window);
        if (menu_window)
        {
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

            Glib::RefPtr<Gdk::Display> display = menu_window->get_display();
            const int num_monitors = display->get_n_monitors();

            builder->get_widget("cbo_monitors", cbo_lista_monitores);
            if (cbo_lista_monitores)
            {
                for (int i = 0; i < num_monitors; i++)
                {
                    cbo_lista_monitores->append(std::to_string(i).c_str());
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
                    cbo_languages_captura->append(i);
                }
            }

            app->run(*menu_window);
        }
    }

    delete menu_window;
    delete main_window;

    return 0;
}

void load_app_config()
{
    std::ifstream file_conf("app_config.json");

    if (file_conf.is_open())
    {
        Json::Reader json_reader;
        std::stringstream buffer;

        buffer << file_conf.rdbuf();
        file_conf.close();
        json_reader.parse(buffer, app_config_params);
    }
}

void list_tessdata()
{
    std::string path = "./traineddata/";

    for (const auto &entry : fs::directory_iterator(path))
    {
        std::string file = entry.path().filename().c_str();

        std::size_t pos = file.find(".traineddata");

        std::string file_no_ext = file.substr(0, pos);

        files_from_tesseract_data.push_back(file_no_ext);

        std::cout << file_no_ext << std::endl;
    }
}