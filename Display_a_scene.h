
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
  namespace ArcGISRuntime {
    class Scene;
    class SceneQuickView;
    class GraphicsOverlay;
    class Graphic;
    class LocatorTask;
    class GeocodeResult;
    class SuggestResult;
    class TextSymbol;
    class Camera;
    class Point;
    namespace Toolkit {
      class NorthArrow;
    }
  }
}

#include <QObject>

#include <QAbstractListModel>
#include "GeocodeParameters.h"

class Display_a_scene : public QObject
{
  Q_OBJECT

  Q_PROPERTY(Esri::ArcGISRuntime::SceneQuickView*
    sceneView READ sceneView WRITE setSceneView NOTIFY sceneViewChanged)
  Q_PROPERTY(QAbstractListModel* suggestions READ suggestions
    NOTIFY suggestionsChanged)

public:
  explicit Display_a_scene(QObject* parent = nullptr);
  ~Display_a_scene() override;
  Q_INVOKABLE void geocode(const QString& query);
  Q_INVOKABLE void clearGraphics();
  Q_INVOKABLE void setSuggestions(const QString& text);

signals:
  void sceneViewChanged();
  void suggestionsChanged();
  void hideSuggestionView();

private:
  Esri::ArcGISRuntime::SceneQuickView* sceneView() const;
  void setSceneView(Esri::ArcGISRuntime::SceneQuickView* sceneView);
  void setupLocatorTask();
  void setupScene();
  Esri::ArcGISRuntime::Scene* m_scene = nullptr;
  Esri::ArcGISRuntime::SceneQuickView* m_sceneView = nullptr;

  void configureGraphic();
  QAbstractListModel* suggestions() const;
  Esri::ArcGISRuntime::GraphicsOverlay* m_graphicsOverlay = nullptr;
  Esri::ArcGISRuntime::LocatorTask* m_locatorTask = nullptr;
  Esri::ArcGISRuntime::Graphic* m_graphicResultLocation = nullptr;
  Esri::ArcGISRuntime::Graphic* m_graphicResultText = nullptr;
  Esri::ArcGISRuntime::Camera* m_graphicResultCamera = nullptr;
  Esri::ArcGISRuntime::TextSymbol* m_textSymbol = nullptr;
  QAbstractListModel* m_suggestListModel = nullptr;
  Esri::ArcGISRuntime::GeocodeParameters m_geocodeParams;
  Esri::ArcGISRuntime::Toolkit::NorthArrow* northArrow = nullptr;

};

#endif // DISPLAY_A_SCENE_H
