#!/bin/bash

echo preparing docs...
@ doxygen Doxyfile

echo opennnig docs...
xdg-open ./docs/html/index.html
