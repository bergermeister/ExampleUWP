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
      std::shared_ptr<DX::DeviceResources> vopDeviceResources;  ///< DirectX device resources
      std::unique_ptr< TcGame >       vopMain;             ///< Main game object

   public:
      App( void );

      // IFrameworkView methods.
      virtual void Initialize( Windows::ApplicationModel::Core::CoreApplicationView^ aopApplicationView );
      virtual void SetWindow( Windows::UI::Core::CoreWindow^ aopWindow );
      virtual void Load( Platform::String^ aopEntryPoint );
      virtual void Run( void );
      virtual void Uninitialize( void );

   protected:
      // Application lifecycle event handlers.
      void mOnActivated( Windows::ApplicationModel::Core::CoreApplicationView^ aopApplicationView, 
                         Windows::ApplicationModel::Activation::IActivatedEventArgs^ aopArgs );
      void mOnSuspending( Platform::Object^ aopSender,
                          Windows::ApplicationModel::SuspendingEventArgs^ aopArgs );
      void mOnResuming( Platform::Object^ aopSender, 
                        Platform::Object^ aopArgs );

      // Window event handlers.
      void mOnWindowSizeChanged( Windows::UI::Core::CoreWindow^ aopSender, 
                                 Windows::UI::Core::WindowSizeChangedEventArgs^ aopArgs );
      void mOnVisibilityChanged( Windows::UI::Core::CoreWindow^ aopSender, 
                                 Windows::UI::Core::VisibilityChangedEventArgs^ aopArgs );
      void mOnWindowClosed( Windows::UI::Core::CoreWindow^ aopSender, 
                            Windows::UI::Core::CoreWindowEventArgs^ aopArgs );
      void mOnWindowActivationChanged( Windows::UI::Core::CoreWindow^ aopSender,
                                       Windows::UI::Core::WindowActivatedEventArgs^ aopArgs );

      // DisplayInformation event handlers.
      void mOnDpiChanged( Windows::Graphics::Display::DisplayInformation^ aopSender, 
                          Platform::Object^ aopArgs );
      void mOnOrientationChanged( Windows::Graphics::Display::DisplayInformation^ aopSender, 
                                  Platform::Object^ aopArgs );
      void mOnDisplayContentsInvalidated( Windows::Graphics::Display::DisplayInformation^ aopSender,
                                          Platform::Object^ aopArgs );
      void mOnStereoEnabledChanged( Windows::Graphics::Display::DisplayInformation^ aopSender,
                                    Platform::Object^ aopArgs );
   };
}

ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
   virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView( );
};
