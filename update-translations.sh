#!/bin/sh

intltool-extract --type="gettext/ini" src/instonusb.desktop.in
intltool-extract --type="gettext/ini" src/instonusb-kde.desktop.in

xgettext --from-code=utf-8 \
	-L Glade \
	-o po/instonusb.pot \
	src/instonusb.ui

xgettext --from-code=utf-8 \
	-j \
	-L Python \
	-o po/instonusb.pot \
	src/instonusb.c
xgettext --from-code=utf-8 -j -L C -kN_ -o po/instonusb.pot src/instonusb.desktop.in.h
xgettext --from-code=utf-8 -j -L C -kN_ -o po/instonusb.pot src/instonusb-kde.desktop.in.h

rm src/instonusb.desktop.in.h src/instonusb-kde.desktop.in.h

cd po
for i in `ls *.po`; do
	msgmerge -U $i instonusb.pot
done
rm -f ./*~
