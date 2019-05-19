# QImageConverter

This is an `XD`-framework based portable Image-converter, which provides the possibility to convert any `Qt4` supported input image-formats to another format which we do select, in a basic and simple way.

![Preview](assets/preview/main-view.png?raw=true "Windows Main preview")

### Resize mode
![Preview](assets/preview/resize-mode-view.png?raw=true "Resize-settings preview")

## Background:
Since `PhotoShop` did not support `SVG` and `WebP` formats (at the time of writing), I was asked to develop a small App that does convert all the `SVG` and `WebP` files in a specified folder to the format which we select.

(source-code: https://github.com/top-master/QImageConverter)


## Build
### pre requirements:
1. **[Qt Creator](https://www.qt.io/offline-installers)** ([github](https://github.com/qt-creator/qt-creator))
2. **[Microsoft Visual C++ 2010 IDE](https://www.google.com/search?q=download%20Microsoft%20Visual%20C%2B%2B%202010%20IDE)** (just used as compiler and not IDE, hence MSVC-Express should be enough, no need to purchase)
3. **[XD framework](https://bitbucket.org/top-master/xd/src/master)**
4. **[Inno Setup 5.6.1 unicode](http://www.jrsoftware.org/isdl.php)** (is optional, to create an Installer)

### steps:
1. Install both '`Qt-Creator`' and '`MSVC 2010`'
(then `Qt-Creator` should automatically detect `MSVC 2010` as compiler when both are installed)

2. Prepare `XD` framework at '`C:/XD`' (download prebuild or build `XD` framework by following its [build instructions](https://bitbucket.org/top-master/xd/src/master/README.md))

3. Open `Qt-Creator`, then [Add Qt-version and configure it](https://bitbucket.org/top-master/xd/src/master/doc/qt-creator/project-qmake.md) like:
   - put in "qmake location" field the `XD` framework's qmake-executable address (e.g. "`C:/XD/bin/qmake.exe`")
   - also set the "`Version name`" field to "`MyXD`"

4. Still in `Qt-Creator` [Create a build tool-kit and configure it](https://bitbucket.org/top-master/xd/src/master/doc/qt-creator/targets.md) like:
   - in "`Qt Version`" Drop-down menu select "`MyXD`" (which you added in previous step)
   - in `"Compiler"` Drop-down menu select the auto-detected one with `x86` architecture (or even [configure compiler](https://bitbucket.org/top-master/xd/src/master/doc/qt-creator/tool-chains.md) manually)

5. In `Qt-Creator` open as project our main build-script file which is "`QImageConverter.pro`" then [configure it](https://bitbucket.org/top-master/xd/src/master/doc/qt-creator/configuring-projects.md)
to use the build tool-kit (which you added in previous step)

6. In `Qt-Creator` press "Build All" from "Build" menu (but before that switch to `release` mode when you have no need for `debug`)


All done, the application is ready to use!
