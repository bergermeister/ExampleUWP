#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include <Common\DeviceResources.h>

// Renders Direct3D content on the screen.
namespace Example
{
   class TcGame : public DX::IDeviceNotify
   {
   private:
      // TODO: Replace with your own content renderers.
      std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;

      // Rendering loop timer.
      DX::StepTimer m_timer;

   public:
      TcGame( const std::shared_ptr< DX::DeviceResources >& korDeviceResources );

      void MCreateWindowSizeDependentResources( void );
      void MRun( void );
      void CreateRenderers(const std::shared_ptr<DX::DeviceResources>& deviceResources);
      void Update();
      bool Render();

      void OnWindowSizeChanged();
      void OnSuspending();
      void OnResuming();
      void OnDeviceRemoved();
   };
}