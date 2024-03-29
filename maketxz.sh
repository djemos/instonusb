#!/bin/sh

cd $(dirname $0)
mkdir -p pkg
export DESTDIR=$PWD/pkg
VER=2.0
ARCH=${ARCH:-x86_64}
RLZ=4dj

	cmake -DCMAKE_INSTALL_PREFIX=/usr .

	make || return 1
	make DESTDIR=pkg install || return 1

	cd pkg

	mkdir install

cat <<EOF > install/slack-desc
instonusb: "Slackel Live USB Installer(Tool to install Slackel Live on USB stick)"
instonusb: "It is  written for Slackel / Slackel64 Linux:"
EOF

/sbin/makepkg -l y -c n ../instonusb-$VER-$ARCH-$RLZ.txz
cd ..
rm -rf pkg
md5sum instonusb-$VER-$ARCH-$RLZ.txz > instonusb-$VER-$ARCH-$RLZ.md5
echo -e "aaa_elflibs|bzip2,aaa_elflibs|expat,aaa_elflibs|libpng,aaa_elflibs|zlib,atk,cairo,fontconfig,freetype,fribidi,gdk-pixbuf2,glib2,graphite2,gtk+2,harfbuzz,libX11,libXau,libXcomposite,libXcursor,libXdamage,libXdmcp,libXext,libXfixes,libXi,libXinerama,libXrandr,libXrender,libXxf86vm,libdrm,libffi,libxcb,libxshmfence,mesa,pango,pcre,pixman,util-linux" > instonusb-$VER-$ARCH-$RLZ.dep
