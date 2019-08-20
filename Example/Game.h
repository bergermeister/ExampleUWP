#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"

// Renders Direct3D content on the screen.
namespace Example
{
   class TcGame : public DX::IDeviceNotify
   {
   private:
      bool                                                m_windowClosed;
      bool                                                m_pauseRequested;
      bool                                                m_pressComplete;
      bool                                                m_renderNeeded;
      bool                                                m_haveFocus;
      bool                                                m_visible;

      std::shared_ptr<DX::DeviceResources>                m_deviceResources;
      Simple3DGameDX::IGameUIControl^ m_uiControl;

      MoveLookController^ m_controller;
      GameRenderer^ m_renderer;
      Simple3DGame^ m_game;

      GameControl::UpdateEngineState                      m_updateState;
      GameControl::UpdateEngineState                      m_updateStateNext;
      GameControl::PressResultState                       m_pressResult;
      GameControl::GameInfoOverlayState                   m_gameInfoOverlayState;
      Simple3DGameDX::GameInfoOverlayCommand              m_gameInfoOverlayCommand;
      uint32                                              m_loadingCount;

   public:
      TcGame( const std::shared_ptr< DX::DeviceResources >& korDeviceResources );

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