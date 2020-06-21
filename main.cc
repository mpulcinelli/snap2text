#include "areacapturewindow.h"
#include "curlreader.h"
#include "drawingareawindow.h"
#include "globals.h"
#include <curl/curl.h>
#include <gtk/gtk.h>
#include <gtk/gtkstyleprovider.h>
#include <gtkmm.h>
#include <gtkmm/enums.h>
#include <sstream>
#include <string.h>

using namespace std;

Glib::RefPtr<Gtk::Builder> builder;
Gtk::Window *main_window = nullptr;
Gtk::Window *menu_window = nullptr;
Gtk::ComboBoxText *cbo_lista_monitores;

static void on_scan_finished(char *texto)
{

    std::string texto_original = trim_copy(std::string(texto));

    if (main_window)
    {
        main_window->hide();
        //
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

            DrawingAreaWindow *d = new DrawingAreaWindow();

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

    CurlReader read;
    char *content_to_send;
    //https://www.googleapis.com/language/translate/v2?key=YOUR-API-KEY&source=en&target=de&q=words+to+translate

    auto x = read.read_url("https://api.github.com/repos/whoshuu/cpr/contributors?anon=true&key=value", content_to_send);

    printf("%s", x.c_str());

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

            app->run(*menu_window);
        }
    }

    delete menu_window;
    delete main_window;

    return 0;
}
