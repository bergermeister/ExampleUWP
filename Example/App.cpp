#include <pch.h>
#include <App.h>

using namespace concurrency;
using namespace DirectX;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::ViewManagement;
using namespace Example;

// The DirectX 12 Application template is documented at https://go.microsoft.com/fwlink/?LinkID=613670&clcid=0x409

// The main function is only used to initialize our IFrameworkView class.
/**
 * @brief
 * Main Function
 *
 * @details
 * @par
 * This function is used to initialize the IFrameworkView object.
 *
 * @param aopArgs    Unused arguments
 *
 * @return
 * This function always returns 0.
 */
[Platform::MTAThread]
int main( Platform::Array< Platform::String^ >^ aopArgs )
{
   auto kopDirect3DApplicationSource = ref new Direct3DApplicationSource( );
   CoreApplication::Run( kopDirect3DApplicationSource );
   return( 0 );
}

/**
 * @brief
 *
 * @details
 * @par
 *
 * @return
 * This function returns a smart pointer to a new Application instance.
 */
IFrameworkView^ Direct3DApplicationSource::CreateView( )
{
   return( ref new App( ) );
}

/**
 * @brief
 * Default Constructor
 *
 * @details
 * @par
 *
 */
App::App( )
{
}

/**
 * @brief
 * Initialize IFrameworkView
 *
 * @details
 * @par
 * This method initializes the IFrameworkView when the application is created.
 *
 * @param aopApplicationView  Pointer to the application view
 *
 * @return
 * This method returns void.
 */
// The first method called when the IFrameworkView is being created.
void App::Initialize( CoreApplicationView^ aopApplicationView )
{
   /// @par Process Design Language
   /// -# Register the Activated event handler for the application lifecycle
   aopApplicationView->Activated +=
      ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>( this, &App::mOnActivated );

   /// -# Register the Suspending event handler for the application lifecycle
   CoreApplication::Suspending +=
      ref new EventHandler<SuspendingEventArgs^>( this, &App::mOnSuspending );

   /// -# Register the Resuming event handler for the application lifecycle
   CoreApplication::Resuming +=
      ref new EventHandler<Platform::Object^>( this, &App::mOnResuming );

   /// -# Obtain pointer to DirectX Device Resources
   this->vopDeviceResources = std::make_shared< DX::DeviceResources >( );
}

/**
 * @brief
 * 
 *
 * @details
 * @par
 * This method is called when the CoreWindow objet is created or re-created.
 */
void App::SetWindow( CoreWindow^ aopWindow )
{
   DisplayInformation^ aopCurrentDisplayInformation;
   PointerVisualizationSettings^ kopVisualizationSettings;

   /// @par Process Design Langauge
   /// -#
   aopWindow->PointerCursor = ref new CoreCursor( CoreCursorType::Arrow, 0 );

   /// -#
   kopVisualizationSettings = PointerVisualizationSettings::GetForCurrentView( );
   kopVisualizationSettings->IsContactFeedbackEnabled = false;
   kopVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;

   this->vopDeviceResources->SetWindow( aopWindow );

   /// -#
   aopWindow->SizeChanged +=
      ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>( this, &App::mOnWindowSizeChanged );

   /// -#
   aopWindow->VisibilityChanged +=
      ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>( this, &App::mOnVisibilityChanged );

   /// -#
   aopWindow->Closed +=
      ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>( this, &App::mOnWindowClosed );

   /// -#
   aopWindow->Activated +=
      ref new TypedEventHandler< CoreWindow^, WindowActivatedEventArgs^ >( this, &App::mOnWindowActivationChanged );

   /// -#
   aopCurrentDisplayInformation = DisplayInformation::GetForCurrentView( );

   /// -#
   aopCurrentDisplayInformation->DpiChanged +=
      ref new TypedEventHandler<DisplayInformation^, Object^>( this, &App::mOnDpiChanged );

   /// -#
   aopCurrentDisplayInformation->OrientationChanged +=
      ref new TypedEventHandler<DisplayInformation^, Object^>( this, &App::mOnOrientationChanged );

   /// -#
   DisplayInformation::DisplayContentsInvalidated +=
      ref new TypedEventHandler<DisplayInformation^, Object^>( this, &App::mOnDisplayContentsInvalidated );
}

/**
 * @brief
 * 
 *
 * @details
 * @par
 * This method initializes scene resources or loads previously saved application state.
 */
void App::Load( Platform::String^ aopEntryPoint )
{
   if( this->vopMain == nullptr )
   {
      this->vopMain = std::unique_ptr<TcGame>( new TcGame( vopDeviceResources ) );
   }
}

/**
 * @brief
 *
 * @details
 * @par
 * This method is called after the window becomes active.
 */
void App::Run( void )
{
   this->vopMain->MRun( );
}

/**
 * @brief
 *
 * @details
 * @par
 * This method is required for the IFrameworkView. It will be called if the IFrameworkView object is torn down while
 * the application is in the foreground. Terminate events do not cause this method to be called.
 */
void App::Uninitialize( void )
{
   /// TODO
}

// Application lifecycle event handlers.

void App::mOnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
   // Run() won't start until the CoreWindow is activated.
   CoreWindow::GetForCurrentThread()->Activate();
}

void App::mOnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
   // Save app state asynchronously after requesting a deferral. Holding a deferral
   // indicates that the application is busy performing suspending operations. Be
   // aware that a deferral may not be held indefinitely. After about five seconds,
   // the app will be forced to exit.
   SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

   create_task([this, deferral]()
   {
      vopMain->OnSuspending();
      deferral->Complete();
   });
}

void App::mOnResuming(Platform::Object^ sender, Platform::Object^ args)
{
   // Restore any data or state that was unloaded on suspend. By default, data
   // and state are persisted when resuming from suspend. Note that this event
   // does not occur if the app was previously terminated.

   vopMain->OnResuming();
}

// Window event handlers.

void App::mOnWindowSizeChanged( CoreWindow^ aopSender, WindowSizeChangedEventArgs^ aopArgs )
{
   ( void )aopSender;
   this->vopDeviceResources->SetLogicalSize( aopArgs->Size );
   this->vopMain->MCreateWindowSizeDependentResources( );
}

void App::mOnVisibilityChanged( CoreWindow^ aopSender, VisibilityChangedEventArgs^ aopArgs )
{
   this->vopMain->MVisibility( aopArgs->Visible );
}

void App::mOnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
   m_windowClosed = true;
}

// DisplayInformation event handlers.

void App::mOnDpiChanged(DisplayInformation^ sender, Object^ args)
{
   // Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
   // if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
   // you should always retrieve it using the GetDpi method.
   // See DeviceResources.cpp for more details.
   GetDeviceResources()->SetDpi(sender->LogicalDpi);
   vopMain->OnWindowSizeChanged();
}

void App::mOnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
   GetDeviceResources()->SetCurrentOrientation(sender->CurrentOrientation);
   vopMain->OnWindowSizeChanged();
}

void App::mOnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
   GetDeviceResources()->ValidateDevice();
}

void App::mOnStereoEnabledChanged( Windows::Graphics::Display::DisplayInformation^ aopSender,
                                   Platform::Object^ aopArgs )
{
   ( void )aopSender;
   ( void )aopArgs;

   this->vopDeviceResources->UpdateStereoState( );
   this->vopMain->MCreateWindowSizeDependentResources( );
}

std::shared_ptr<DX::DeviceResources> App::GetDeviceResources()
{
   if (voDeviceResources != nullptr && voDeviceResources->IsDeviceRemoved())
   {
      // All references to the existing D3D device must be released before a new device
      // can be created.

      voDeviceResources = nullptr;
      voMain->OnDeviceRemoved();

#if defined(_DEBUG)
      ComPtr<IDXGIDebug1> dxgiDebug;
      if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
      {
         dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
      }
#endif
   }

   if (voDeviceResources == nullptr)
   {
      voDeviceResources = std::make_shared<DX::DeviceResources>();
      voDeviceResources->SetWindow(CoreWindow::GetForCurrentThread());
      voMain->CreateRenderers(voDeviceResources);
   }
   return voDeviceResources;
}
