
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

#include "ArcGISTiledElevationSource.h"
#include "Basemap.h"
#include "Scene.h"
#include "SceneQuickView.h"

#include <QUrl>

using namespace Esri::ArcGISRuntime;

Display_a_scene::Display_a_scene(QObject* parent /* = nullptr */):
    QObject(parent),
    m_scene(new Scene(BasemapStyle::ArcGISImagery, this))
{
    // create a new elevation source from Terrain3D rest service
    ArcGISTiledElevationSource* elevationSource = new ArcGISTiledElevationSource(
                QUrl("https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer"), this);

    // add the elevation source to the scene to display elevation
    m_scene->baseSurface()->elevationSources()->append(elevationSource);
}

Display_a_scene::~Display_a_scene()
{
}

SceneQuickView* Display_a_scene::sceneView() const
{
    return m_sceneView;
}

// Set the view (created in QML)
void Display_a_scene::setSceneView(SceneQuickView* sceneView)
{
    if (!sceneView || sceneView == m_sceneView)
    {
        return;
    }

    m_sceneView = sceneView;
    m_sceneView->setArcGISScene(m_scene);

    emit sceneViewChanged();
}
