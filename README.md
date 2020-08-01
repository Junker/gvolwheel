# GVolWheel
GVolWheel is a Lightweight application which lets you control the audio volume.
GVolWheel is application which lets you control the volume easily through a tray icon you can scroll on. 
Easily integrate with minimal desktops (Openbox,IceWM,XFCE etc).

# Install:
```
./autogen.sh
./configure  # for ALSA
./configure --enable-oss # for OSS

make

make install
```


### Pixmaps:
($prefix)/share/pixmaps/gvolwheel/audio-volume-low.png
($prefix)/share/pixmaps/gvolwheel/audio-volume-medium.png
($prefix)/share/pixmaps/gvolwheel/audio-volume-high.png
($prefix)/share/pixmaps/gvolwheel/audio-volume-muted.png

### Own pixmaps:
~/.config/gvolwheel/audio-volume-low.png
~/.config/gvolwheel/audio-volume-medium.png
~/.config/gvolwheel/audio-volume-high.png
~/.config/gvolwheel/audio-volume-muted.png


You can change it. Just replace pixmaps from any theme. (default icons path: /usr/share/icons/$theme/24x24/status/)

For use gtk theme icons, open "Preferences" and change "Use gnome icons"
