#include "drawingarea.h"
#include <gtkmm.h>

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "br.com.snap2text");

    Gtk::Window win;
    win.set_title("DrawingArea");

    DrawingArea d;

    win.add(d);
    d.show_all();
    win.fullscreen();

    return app->run(win);
}