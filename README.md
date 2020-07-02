# Application Snap2texT

This project aims be an application to take screenshots from images (containing text), convert them to pure text and translate the text to other languages.

## Choose the text you need to capture.



## Capture the text



## Choose the language to translate and click translate



# Build From Source Code

/bin/g++ -g ./\*.cc -o ./bin/snap2text `pkg-config --libs --cflags lept tesseract gtkmm-3.0 curlpp jsoncpp gtk+-3.0` `curl-config --libs --cflags`

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

# Libs Installation

### glib2.0

```
sudo apt-get install glib2.0
```

### gtkmm-3.0

```
sudo apt-get install libgtkmm-3.0-dev
```

### glibmm-2.4

```
sudo apt-get install libglibmm-2.4-dev
```

### gtk-3.0

```
sudo apt-get install libgtk-3-dev
sudo apt-get install appmenu-gtk3-module
```

### tesseract

```
sudo apt-get install tesseract-ocr
sudo apt-get install libtesseract-dev
```

### leptonica

```
sudo apt-get install libleptonica-dev
```

### curl

```
sudo apt-get install curl
sudo apt-get install libcurl4-openssl-dev
```

### curlpp

```
sudo apt-get install libcurlpp-dev
```

### jsoncpp

```
sudo apt-get install libjsoncpp-dev
```
