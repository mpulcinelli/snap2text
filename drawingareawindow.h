#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <leptonica/allheaders.h>

#include <gtkmm.h>
#include <gtkmm/enums.h>
#include <tesseract/baseapi.h>
#include <vector>

class DrawingAreaWindow : public Gtk::DrawingArea
{
public:
    DrawingAreaWindow(int monitor, Glib::ustring language_ocr);
    virtual ~DrawingAreaWindow();
    typedef sigc::signal<void, std::string> type_signal_on_scan_finish;
    type_signal_on_scan_finish signal_on_scan_finish();

protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
    virtual bool on_motion_notify_event(GdkEventMotion *motion_event) override;
    virtual bool on_focus(Gtk::DirectionType direction) override;
    bool on_button_press_event(GdkEventButton *event) override;

    type_signal_on_scan_finish m_signal_on_scan_finish;

    void get_screen_pixels();

private:
    //display Pixbuf
    Glib::RefPtr<Gdk::Pixbuf> pixels;
    Gdk::Rectangle rect;

    //Coordenadas para o mouse na tela
    int x1;
    int y1;
    int x2;
    int y2;

    int m_heigth;
    int m_width;

    //bools para os clicks da tela.
    bool firstclick;
    bool secondclick;
    void take_screen_shot();
    void get_text_from_screen_shot();
    int active_monitor = 0;
    Glib::ustring active_language_ocr;
};
#endif // DRAWINGAREA_H