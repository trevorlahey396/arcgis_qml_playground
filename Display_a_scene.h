
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

#ifndef DISPLAY_A_SCENE_H
#define DISPLAY_A_SCENE_H

namespace Esri
{
namespace ArcGISRuntime
{
class Scene;
class SceneQuickView;
}
}

#include <QObject>

class Display_a_scene : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Esri::ArcGISRuntime::SceneQuickView* sceneView READ sceneView WRITE setSceneView NOTIFY sceneViewChanged)

public:
    explicit Display_a_scene(QObject* parent = nullptr);
    ~Display_a_scene() override;

signals:
    void sceneViewChanged();

private:
    Esri::ArcGISRuntime::SceneQuickView* sceneView() const;
    void setSceneView(Esri::ArcGISRuntime::SceneQuickView* sceneView);

    Esri::ArcGISRuntime::Scene* m_scene = nullptr;
    Esri::ArcGISRuntime::SceneQuickView* m_sceneView = nullptr;
};

#endif // DISPLAY_A_SCENE_H
