Create AppImage of Spotweb Filter Creator.

1. Place application binaries + qm translations into the /usr/bin folder

2. Download: https://github.com/probonopd/linuxdeployqt/releases/download/5/linuxdeployqt-5-x86_64.AppImage

3. Run: chmod a+x ./linuxdeployqt-5-x86_64.AppImage

4. Move linuxdeployqt-5-x86_64.AppImage to tools folder.

5. Run:
../tools/linuxdeployqt-5-x86_64.AppImage AppDir/usr/share/applications/spotwebfc.desktop -appimage -extra-plugins=iconengines,platformthemes/libqgtk3.so -no-translations -verbose=2

