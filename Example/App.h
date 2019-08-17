#pragma once

#include <Common\DeviceResources.h>
#include <ExampleMain.h>

/// Example UWP Game Namespace
namespace Example
{
   /**
    * @brief
    * Main entry point
    *
    * @details
    * @par
    * This class is the main entry point for the Example Game. It Connects the game with the Windows shell and handles application
    * lifecycle events.
    */
   ref class App sealed : public Windows::ApplicationModel::Core::IFrameworkView
   {
   private: // Private Attributes
      // Private accessor for m_deviceResources, protects against device removed errors.
      std::shared_ptr<DX::DeviceResources> voDeviceResources;  ///< DirectX device resources
      std::unique_ptr< ExampleMain >       voMain;             ///< Main game object

   public:
      App( );

      // IFrameworkView methods.
      virtual void Initialize( Windows::ApplicationModel::Core::CoreApplicationView^ applicationView );
      virtual void SetWindow( Windows::UI::Core::CoreWindow^ window );
      virtual void Load( Platform::String^ entryPoint );
      virtual void Run( );
      virtual void Uninitialize( );

   protected:
      // Application lifecycle event handlers.
      void OnActivated( Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, 
                        Windows::ApplicationModel::Activation::IActivatedEventArgs^ args );
      void OnSuspending( Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args );
      void OnResuming(Platform::Object^ sender, Platform::Object^ args);

      // Window event handlers.
      void OnWindowSizeChanged( Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args );
      void OnVisibilityChanged( Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args );
      void OnWindowClosed( Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args );

      // DisplayInformation event handlers.
      void OnDpiChanged( Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args );
      void OnOrientationChanged( Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args );
      void OnDisplayContentsInvalidated( Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args );
   };
}

ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
   virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView( );
};
