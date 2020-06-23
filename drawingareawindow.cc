#include "drawingareawindow.h"
#include "globals.h"
#include "menuappwindow.h"

using namespace std;

DrawingAreaWindow::DrawingAreaWindow(int monitor)
{
    active_monitor = monitor;

    add_events(Gdk::BUTTON_PRESS_MASK);
    add_events(Gdk::POINTER_MOTION_MASK);

    firstclick = false;
    secondclick = false;
    get_screen_pixels();
}

bool DrawingAreaWindow::on_button_press_event(GdkEventButton *event)
{
    if ((event->type == GDK_BUTTON_PRESS) && (event->button == 1))
    {
        if (!firstclick && !secondclick)
        {
            x1 = event->x;
            y1 = event->y;
            firstclick = true;
        }

        if (firstclick && !secondclick && (int)event->x != x1 && (int)event->y != y1)
        {
            x2 = event->x;
            y2 = event->y;
            secondclick = true;
            take_screen_shot();
            queue_draw();
        }

        return true;
    }

    return false;
}

/** 
 * Método para copiar os pixels do monitor.
*/
void DrawingAreaWindow::get_screen_pixels()
{
    // get_default_root_window pega a tela completa.
    auto root = Gdk::Window::get_default_root_window();
    int height = root->get_height();
    int width = root->get_width();

    auto display = root->get_display();
    auto screen = display->get_screen();

    screen->get_monitor_geometry(active_monitor, rect);
    pixels = Gdk::Pixbuf::create(root, rect.get_x(), rect.get_y(), rect.get_width(), rect.get_height());
}

/** 
 * Evento para renderizar os pixels na tela
*/
bool DrawingAreaWindow::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{

    if (!firstclick)
    {
        Gdk::Cairo::set_source_pixbuf(cr, pixels);
        cr->rectangle(0, 0, rect.get_width(), rect.get_height());
        cr->fill();
    }

    //check whether it was clicked two times
    if (firstclick)
    {
        // Preenche o background.
        Gdk::Cairo::set_source_pixbuf(cr, pixels);
        cr->rectangle(0, 0, rect.get_width(), rect.get_height());
        cr->fill();

        // Preenche o retângulo.
        cr->set_line_width(1);
        cr->set_source_rgba(255, 0, 0, 1);
        cr->rectangle(x1, y1, m_width, m_heigth);
        cr->stroke();
    }

    return true;
}

bool DrawingAreaWindow::on_motion_notify_event(GdkEventMotion *motion_event)
{
    m_heigth = motion_event->y - y1;
    m_width = motion_event->x - x1;

    if (firstclick && !secondclick)
    {
        auto win = get_window();
        if (win)
        {
            Gdk::Rectangle r(0, 0, get_allocation().get_width(), get_allocation().get_height());
            win->invalidate_rect(r, false);
        }
    }

    return TRUE;
}

bool DrawingAreaWindow::on_focus(Gtk::DirectionType direction)
{
    return true;
}

void DrawingAreaWindow::take_screen_shot()
{
    Glib::RefPtr<Gdk::Window> root_window = this->get_parent_window();
    pixels = Gdk::Pixbuf::create(root_window, x1, y1, m_width, m_heigth);
    get_text_from_screen_shot();
}

void DrawingAreaWindow::get_text_from_screen_shot()
{
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();

    const char *datapath = "./traineddata/";

    if (ocr->Init(datapath, "eng"))
    {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    ocr->SetPageSegMode(tesseract::PSM_AUTO);

    /////////////////////////////////////////////////////////////////////
    /* GAMBIARRA - O ideal seria pegar o objeto pixels (Glib::RefPtr<Gdk::Pixbuf>) e converter para Pix*
        - SetImage(Pix* pix); ou
        - void SetImage(const unsigned char* imagedata, int width, int height,
                int bytes_per_pixel, int bytes_per_line);        
    */

    pixels->save("./file.png", "png");

    Pix *image = pixRead("./file.png");

    ocr->SetImage(image);

    /////////////////////////////////////////////////////////////////////

    text_result_from_scan = ocr->GetUTF8Text();

    ocr->End();

    m_signal_on_scan_finish.emit(text_result_from_scan);
}

DrawingAreaWindow::type_signal_on_scan_finish DrawingAreaWindow::signal_on_scan_finish()
{
    return m_signal_on_scan_finish;
}

DrawingAreaWindow::~DrawingAreaWindow() {}
