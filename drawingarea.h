#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <leptonica/allheaders.h>

#include <gtkmm.h>
#include <gtkmm/enums.h>
#include <tesseract/baseapi.h>
#include <vector>

class DrawingArea : public Gtk::DrawingArea
{
public:
    DrawingArea();
    virtual ~DrawingArea();

protected:
    virtual bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr);
    virtual bool on_motion_notify_event(GdkEventMotion *motion_event);
    virtual bool on_focus(Gtk::DirectionType direction);
    Gtk::Window *_menu;
    bool on_button_press_event(GdkEventButton *event);
    //Gtk::Window *_menu;

private:
    //display Pixbuf
    Glib::RefPtr<Gdk::Pixbuf> pixels;
    Gdk::Rectangle rect;

    //two coordinates
    int x1;
    int y1;
    int x2;
    int y2;

    int m_heigth;
    int m_width;
    //two bools for the clicks
    bool firstclick;
    bool secondclick;
    void take_screen_shot();
    void get_text_from_screen_shot();
};
#endif // DRAWINGAREA_H