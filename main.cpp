
// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#include "Display_a_scene.h"

#include "ArcGISRuntimeEnvironment.h"
#include "SceneQuickView.h"

#include <iostream>
#include <fstream>
#include <string>

#include <QDir>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QQmlApplicationEngine>

//--------------------------------------------------------------------------

using namespace Esri::ArcGISRuntime;
using namespace std;

int main(int argc, char *argv[])
{

#if defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)
    // Linux requires 3.2 OpenGL Context
    // in order to instance 3D symbols
    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();
    fmt.setVersion(3, 2);
    QSurfaceFormat::setDefaultFormat(fmt);
#endif

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    string api;
//    fstream f;
//    f.open("test.txt", ios::in);
    ifstream f ("../.env");
    if (f.is_open()) {
      getline(f, api);
      std::cout << "Read api key" << std::endl;
    } else {
      std::cout << "Unable to open .env file!" << std::endl;
    }
    f.close();

    const QString apiKey = QString::fromStdString(api);
    if (apiKey.isEmpty())
    {
        qWarning() << "Missing API-Key!";
    }
    else
    {
        ArcGISRuntimeEnvironment::setApiKey(apiKey);
    }

    // Register the scene view for QML
    qmlRegisterType<SceneQuickView>(
        "Esri.display_a_scene", 1, 0, "SceneView");

    // Register the Display_a_scene (QQuickItem) for QML
    qmlRegisterType<Display_a_scene>(
        "Esri.display_a_scene", 1, 0, "Display_a_scene");

    // Initialize application view
    QQmlApplicationEngine engine;

    // Add the import Path
    engine.addImportPath(
        QDir(QCoreApplication::applicationDirPath()).filePath("qml"));

    // Set the source
    engine.load(QUrl("qrc:/qml/main.qml"));

    return app.exec();
}

//--------------------------------------------------------------------------
