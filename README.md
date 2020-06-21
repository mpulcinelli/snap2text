# Application Snap2texT

This project aims be an application to take screenshots from images (containing text), convert them to pure text and translate the text to other languages.



# Build From Source Code

/bin/g++ -g ./*.cc -o ./bin/snap2text `pkg-config --libs --cflags lept tesseract gtkmm-3.0 curlpp jsoncpp gtk+-3.0` `curl-config --libs --cflags`



# Used Libs

- glib-2.0

- gtkmm-3.0

- glibmm-2.4

- gtk-3.0

- tesseract

- leptonica

- curl

- curlpp

- jsoncpp
