# QImageConverter

This is an `XD`-framework based portable Image-converter,
which provides the possibility to convert from any `Qt4` supported input image-formats,
to the other format which we do select, in a basic and simple way.

![Preview](assets/preview/main-view.png?raw=true "Windows Main preview")

### Resize mode
![Preview](assets/preview/resize-mode-view.png?raw=true "Resize-settings preview")

## Background:
Since `PhotoShop` did not support `SVG` and `WebP` formats (at the time of writing), I was asked to develop a small App that does convert all the `SVG` and `WebP` files in a specified folder to the format which we select.

(source-code: https://github.com/top-master/QImageConverter)


## Build
### Pre-requisite:
1. **[Qt Creator](https://www.qt.io/offline-installers)** ([github](https://github.com/qt-creator/qt-creator))
2. **[MSVC 2015 (Microsoft Visual Studio 2015)](https://www.google.com/search?q=download%20Microsoft%20Visual%20Studio%202015%20Community)**
    - Just used as compiler, not as IDE, hence installing should be enough (no need to purchase).
3. **[XD framework](https://bitbucket.org/top-master/xd/src/master)**
4. **[Git](https://git-scm.com/downloads)**
5. **[Inno Setup 5.6.1 unicode](http://www.jrsoftware.org/isdl.php)** (is optional, to create an Installer)

### steps:
1. Install both '`Qt-Creator`' and '`MSVC 2015`'
(then `Qt-Creator` should automatically detect `MSVC 2015` as compiler, once both are installed)

2. Prepare `XD` framework at '`C:/XD`' (download prebuilt, or, build `XD` framework by following its [build instructions](https://bitbucket.org/top-master/xd/src/master/README.md))

3. Open `Qt-Creator`, then [Add Qt-version and configure it](https://bitbucket.org/top-master/xd/src/master/doc/qt-creator/project-qmake.md) like:
    - Put in "`qmake location`" field the `XD` framework's qmake-executable address (e.g. "`C:/XD/bin/qmake.exe`")
    - Also, set the "`Version name`" field to "`MyXD`"

4. Still in `Qt-Creator` [Create a build tool-kit and configure it](https://bitbucket.org/top-master/xd/src/master/doc/qt-creator/targets.md) like:
    - In "`Qt Version`" Drop-down menu, select "`MyXD`" (which you added in previous step)
    - In "`Compiler`" Drop-down menu select the auto-detected one with `x86` architecture (or even [configure compiler](https://bitbucket.org/top-master/xd/src/master/doc/qt-creator/tool-chains.md) manually)

5. In `Qt-Creator` open as project our main build-script file which is the "`QImageConverter.pro`" file, then [configure it](https://bitbucket.org/top-master/xd/src/master/doc/qt-creator/configuring-projects.md)
to use the build tool-kit (which you added in previous step)

6. In `Qt-Creator` select "Build All" from "Build" menu (but before that, switch to `release` mode if you have no need for `debug`, using `Ctrl+T` menu).

7. Finally, wait till all components are built (but in case of failure see `Issues` pane; using `Alt+1` shortcut, which lists any problem or warning).


All done, the application is ready to use!
