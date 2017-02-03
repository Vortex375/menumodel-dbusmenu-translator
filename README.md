---
MenuModel - DBusMenu tranlator.
---

This is a small daemon to translate Unity's MenuModels to KDE DBusMenus.

*Dependencies:*

*All:*
 * GLib (>= 2.40.0)
 * valac (>= 0.24.0)

---
*Installation after building*
---
* Create a .gtkrc in your home(~) directory
* Create a .config/gtk-3.0/settings.ini file in your home(~) directory
* Add to this files:
`export   GTK_MODULES=unity-gtk-module:$GTK_MODULES`
* Add to .config/gtk-3.0/settings.ini file only:
`gtk-shell-shows-app-menu=true`
`gtk-shell-shows-menubar=true`
* Add to ~/.profile:
`export UBUNTU_MENUPROXY=libappmenu.so`

*Arch Linux*
* Install from AUR [unity-gtk-module-standalone-bzr](https://aur.archlinux.org/packages/unity-gtk-module-standalone-bzr/) for appmenu to work
* Install these [libdbusmenu-glib](https://aur.archlinux.org/packages/libdbusmenu-glib/) [libdbusmenu-gtk3](https://aur.archlinux.org/packages/libdbusmenu-gtk3/) [libdbusmenu-gtk2](https://aur.archlinux.org/packages/libdbusmenu-gtk2/) to get Chromium/Google Chrome to work

*Building unity-gtk-module from sources for distros other than Arch or Ubuntu*
* `bzr branch lp:unity-gtk-module` 
* `git clone https://github.com/rilian-la-te/unity-gtk-module-packages.git`
* Copy a patch from github package to unity-gtk-module branch
* `patch -p0 -i "unity-gtk-module-gsettings.patch"`
* `../configure --prefix=/usr --sysconfdir=/etc --with-gtk=2 --enable-gtk-doc --disable-static && make && sudo make install &&   ../configure --prefix=/usr --sysconfdir=/etc --with-gtk=3 --enable-gtk-doc --disable-static && make && sudo make install`

Author
===
 * Athor <ria.freelander@gmail.com>
