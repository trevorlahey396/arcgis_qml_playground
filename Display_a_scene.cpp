
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
#include "SimpleRenderer.h"
#include "SimpleMarkerSymbol.h"
#include "TextSymbol.h"
#include "LocatorTask.h"
#include "SuggestResult.h"
#include "SuggestListModel.h"
#include <QUrl>
#include <iostream>
#include <string>
#include <QAbstractListModel>
#include <QGeoPositionInfoSource>

using namespace Esri::ArcGISRuntime;

Display_a_scene::Display_a_scene(QObject* parent /* = nullptr */):
    QObject(parent),
    m_scene(new Scene(BasemapStyle::ArcGISImagery, this))
{
    std::string imServer = "https://elevation3d.arcgis.com/arcgis/rest/"
                           "services/WorldElevation3D/Terrain3D/ImageServer";
    QString imServerQ = QString::fromStdString(imServer);
    // create a new elevation source from Terrain3D rest service
    auto elevationSource = new ArcGISTiledElevationSource(QUrl(imServerQ));

    // add the elevation source to the scene to display elevation
    m_scene->baseSurface()->elevationSources()->append(elevationSource);
    setupLocatorTask();
}

Display_a_scene::~Display_a_scene()
{
}

void Display_a_scene::setupLocatorTask() {
  std::string geoServer = "https://geocode.arcgis.com/arcgis/rest/"
                         "services/World/GeocodeServer";
  QString geoServerQ = QString::fromStdString(geoServer);
  const QUrl geocode_service(geoServerQ);
  m_locatorTask = new LocatorTask(geocode_service, this);

  SuggestParameters suggestParams;
  const QStringList categories{"address", "POI", "Populated Place"};
  suggestParams.setCategories(categories);
  suggestParams.setMaxResults(5);
  m_locatorTask->suggestions()->setSuggestParameters(suggestParams);

  m_geocodeParams.setMinScore(75);
  const QStringList resNames{"Place_addr", "Match_addr"};
  m_geocodeParams.setResultAttributeNames(resNames);
  m_suggestListModel = m_locatorTask->suggestions();
  emit suggestionsChanged();

  connect(m_locatorTask, &LocatorTask::geocodeCompleted, this,
          [this](QUuid, const QList<GeocodeResult>& geocodeResults) {
    if (!geocodeResults.isEmpty() && m_graphicResultLocation) {
      const GeocodeResult geocodeResult = geocodeResults.at(0);
      m_graphicResultLocation->setGeometry(geocodeResult.displayLocation());
      const Point here = geocodeResult.displayLocation();
      const double lat = here.y();
      const double lon = here.x();
      const double alt = 500;
      const double head = 0;
      const double pitch = 0;
      const double roll = 0;
      Camera cam(lat, lon, alt, head, pitch, roll);
      m_sceneView->setViewpointCameraAndWait(cam);
    }
  });
}

SceneQuickView* Display_a_scene::sceneView() const
{
    return m_sceneView;
}

void Display_a_scene::setupScene() {
  connect(m_sceneView, &SceneQuickView::mousePressed, this, [this](
      QMouseEvent&) {
    emit hideSuggestionView();
  });
  const double lat = 55.699164040801094;
  const double lon = 12.533241178291192;
  const double alt = 500;
  const double head = 0;
  const double pitch = 0;
  const double roll = 0;
  Camera cam(lat, lon, alt, head, pitch, roll);
  m_sceneView->setViewpointCameraAndWait(cam);
  configureGraphic();
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

    setupScene();

    emit sceneViewChanged();
}

void Display_a_scene::setSuggestions(const QString& text) {
  if (!m_suggestListModel) {
    return;
  }
  SuggestListModel* suggestListModel = dynamic_cast<SuggestListModel*>(
      m_suggestListModel);
  if (!suggestListModel) {
    return;
  }
  suggestListModel->setSearchText(text);
}

void Display_a_scene::configureGraphic() {
  if (m_graphicResultLocation) {
    return;
  }
  m_graphicsOverlay = new GraphicsOverlay(this);

  // set a renderer on the graphics overlay
  SimpleRenderer* simpleRenderer = new SimpleRenderer(this);

  // Create a graphic and symbol to display the result location.
  SimpleMarkerSymbol* simpleMarkerSymbol = new SimpleMarkerSymbol(
      SimpleMarkerSymbolStyle::Square, QColor(0, 0, 255, 63), 12.0, this);
  simpleRenderer->setSymbol(simpleMarkerSymbol);
  m_graphicResultLocation = new Graphic(this);
  m_graphicResultLocation->setSymbol(simpleMarkerSymbol);
  m_graphicsOverlay->graphics()->append(m_graphicResultLocation);

  // Create a graphic and symbol to display a label next to the result location
  m_textSymbol = new TextSymbol("", QColor("red"), 18.0,
    HorizontalAlignment::Center, VerticalAlignment::Bottom, this);
  m_graphicResultText = new Graphic(this);
  m_graphicResultText->setSymbol(m_textSymbol);

  m_graphicsOverlay->graphics()->append(m_graphicResultText);

  m_sceneView->graphicsOverlays()->append(m_graphicsOverlay);
}

void Display_a_scene::geocode(const QString& query) {
  m_locatorTask->geocodeWithParameters(query, m_geocodeParams);
}

QAbstractListModel* Display_a_scene::suggestions() const {
  return m_suggestListModel;
}

void Display_a_scene::clearGraphics() {
  return;
}
