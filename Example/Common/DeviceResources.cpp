#include <pch.h>
#include <Common/DeviceResources.h>
#include <Common/DirectXHelper.h>

using namespace D2D1;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Controls;
using namespace Platform;

// Constants used to calculate screen rotations.
namespace ScreenRotation
{
   // 0-degree Z-rotation
   static const XMFLOAT4X4 Rotation0( 1.0f, 0.0f, 0.0f, 0.0f, 
                                      0.0f, 1.0f, 0.0f, 0.0f, 
                                      0.0f, 0.0f, 1.0f, 0.0f, 
                                      0.0f, 0.0f, 0.0f, 1.0f );

   // 90-degree Z-rotation
   static const XMFLOAT4X4 Rotation90(  0.0f, 1.0f, 0.0f, 0.0f,
                                       -1.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 1.0f );

   // 180-degree Z-rotation
   static const XMFLOAT4X4 Rotation180( -1.0f,  0.0f, 0.0f, 0.0f,
                                         0.0f, -1.0f, 0.0f, 0.0f,
                                         0.0f,  0.0f, 1.0f, 0.0f,
                                         0.0f,  0.0f, 0.0f, 1.0f );

   // 270-degree Z-rotation
   static const XMFLOAT4X4 Rotation270( 0.0f, -1.0f, 0.0f, 0.0f,
                                        1.0f,  0.0f, 0.0f, 0.0f,
                                        0.0f,  0.0f, 1.0f, 0.0f,
                                        0.0f,  0.0f, 0.0f, 1.0f );
};

/**
 * @brief
 * Default Constructor
 *
 * @details
 * @par
 *
 */
// Constructor for DeviceResources.
DX::DeviceResources::DeviceResources( void ) : voScreenViewport( ),
                                               veD3DFeatureLevel( D3D_FEATURE_LEVEL_9_1 ),
                                               voD3DRenderTargetSize( ),
                                               voOutputSize( ),
                                               voLogicalSize( ),
                                               veNativeOrientation( DisplayOrientations::None ),
                                               veCurrentOrientation( DisplayOrientations::None ),
                                               vfDPI( -1.0f ),
                                               vopDeviceNotify( nullptr )
{
   this->mCreateDeviceIndependentResources( );
   this->mCreateDeviceResources( );
}

/**
 * @brief
 * Destructor
 *
 * @details
 * @par
 *
 */
DX::DeviceResources::~DeviceResources( void )
{
   // TODO
}

/**
 * @brief
 * 
 *
 * @details
 * @par
 * This method is called when the CoreWindow is created or re-created.
 */
void DX::DeviceResources::MSetWindow( Windows::UI::Core::CoreWindow^ aopWindow )
{
   DisplayInformation^ kopCurrentDisplayInformation = DisplayInformation::GetForCurrentView( );

   this->vopWindow = aopWindow;
   this->voLogicalSize = Windows::Foundation::Size( aopWindow->Bounds.Width, aopWindow->Bounds.Height );
   this->veNativeOrientation = kopCurrentDisplayInformation->NativeOrientation;
   this->veCurrentOrientation = kopCurrentDisplayInformation->CurrentOrientation;
   this->vfDPI = kopCurrentDisplayInformation->LogicalDpi;
   this->vopD2DContext->SetDpi( this->vfDPI, this->vfDPI );

   this->mCreateWindowSizeDependentResources( );
}

/**
 * @brief
 * 
 *
 * @details
 * @par
 * This method is called in the event handler for the SizeChanged event.
 */
void DX::DeviceResources::MSetLogicalSize( Windows::Foundation::Size aoLogicalSize )
{
   if( this->voLogicalSize != aoLogicalSize )
   {
      this->voLogicalSize = aoLogicalSize;
      this->mCreateWindowSizeDependentResources( );
   }
}

/**
 * @brief
 *
 * @details
 * @par
 * This method is called in the event handler for the OrientationChanged event.
 */
void DX::DeviceResources::MSetCurrentOrientation( Windows::Graphics::Display::DisplayOrientations aeCurrentOrientation )
{
   if( this->veCurrentOrientation != aeCurrentOrientation )
   {
      this->veCurrentOrientation = aeCurrentOrientation;
      this->mCreateWindowSizeDependentResources( );
   }
}

/**
 * @brief
 *
 * @details
 * @par
 * This method is called in the event handler for the DpiChanged event.
 */
void DX::DeviceResources::MSetDPI( float afDPI )
{
   if( this->vfDPI != afDPI )
   {
      this->vfDPI = afDPI;

      // When the display DPI changes, the logical size of the winow (measured in Dips) also changes and needs to be updated
      this->voLogicalSize = Windows::Foundation::Size( this->vopWindow->Bounds.Width, this->vopWindow->Bounds.Height );

      this->vopD2DContext->SetDpi( this->vfDPI, this->vfDPI );
      this->mCreateWindowSizeDependentResources( );
   }
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
void DX::DeviceResources::MUpdateStereoState( void )
{
   bool kbPreviousStereoState = this->vbStereoEnabled;
   
   this->mCheckStereoEnabledStatus( );
   if( kbPreviousStereoState != this->vbStereoEnabled )
   {
      // Swap chain needs to be recreated so release the existing one.
      // The rest of the dependent resurces with be released in CreateWindowSizeDependentResources.
      this->vopSwapChain = nullptr;
      this->mCreateWindowSizeDependentResources( );
   }
}

/**
 * @brief
 *
 *
 * @details
 * @par
 * This method is called in the event handler for the DisplayContentsInvalidated event.
 *
 * @note
 * The D3D Device is no longer valid if the default adapter changed since the device was created or if the device has been removed.
 */
void DX::DeviceResources::MValidateDevice( void )
{
   ComPtr< IDXGIDevice3 >  koDXGIDevice;
   ComPtr< IDXGIAdapter >  koDeviceAdapter;
   ComPtr< IDXGIFactory2 > koDeviceFactory;
   ComPtr< IDXGIAdapter1 > koPreviousDefaultAdapter;
   DXGI_ADAPTER_DESC       koPreviousDesc;
   ComPtr< IDXGIFactory2 > koCurrentFactory;
   ComPtr< IDXGIAdapter1 > koCurrentDefaultAdapter;
   DXGI_ADAPTER_DESC       koCurrentDesc;

   /// -# Get the infiromation for the default adapter from when the evice was created.
   DX::ThrowIfFailed( this->vopD3DDevice.As( &koDXGIDevice ) );
   DX::ThrowIfFailed( koDXGIDevice->GetAdapter( &koDeviceAdapter ) );
   DX::ThrowIfFailed( koDeviceAdapter->GetParent( IID_PPV_ARGS( &koDeviceFactory ) ) );
   DX::ThrowIfFailed( koDeviceFactory->EnumAdapters1( 0, &koPreviousDefaultAdapter ) );
   DX::ThrowIfFailed( koPreviousDefaultAdapter->GetDesc( &koPreviousDesc ) );

   /// -# Get the information for the current default adapter
   DX::ThrowIfFailed( CreateDXGIFactory1( IID_PPV_ARGS( &koCurrentFactory ) ) );
   DX::ThrowIfFailed( koCurrentFactory->EnumAdapters1( 0, &koCurrentDefaultAdapter ) );
   DX::ThrowIfFailed( koCurrentDefaultAdapter->GetDesc( &koCurrentDesc ) );

   /// -# If the adapter LUIDs don't match, or if the device reports that it has been removed, a new D3D device must be created
   if( ( koPreviousDesc.AdapterLuid.LowPart  != koCurrentDesc.AdapterLuid.LowPart  ) ||
       ( koPreviousDesc.AdapterLuid.HighPart != koCurrentDesc.AdapterLuid.HighPart ) )
   {
      /// -# Release references to resources related to the old device
      koDXGIDevice             = nullptr;
      koDeviceAdapter          = nullptr;
      koDeviceFactory          = nullptr;
      koPreviousDefaultAdapter = nullptr;

      /// -# Create a new device and swap chain
      this->MHandleDeviceLost( );
   }
}

/**
 * @brief
 *
 *
 * @details
 * @par
 * This method recreates all device resources and sets them back to the current state.
 */
void DX::DeviceResources::MHandleDeviceLost( void )
{
   this->vopSwapChain = nullptr;

   if( this->vopDeviceNotify != nullptr )
   {
      this->vopDeviceNotify->MOnDeviceLost( );
   }

   // Make sure the rendering state has been released
   this->vopD3DContext->OMSetRenderTargets( 0, nullptr, nullptr );
   this->vopD3DDepthStencilView      = nullptr;
   this->vopD3DRenderTargetView      = nullptr;
   this->vopD3DRenderTargetViewRight = nullptr;

   this->vopD2DContext->SetTarget( nullptr );
   this->vopD2DTargetBitmap      = nullptr;
   this->vopD2DTargetBitmapRight = nullptr;
   this->vopD2DContext           = nullptr;
   this->vopD2DDevice            = nullptr;

   this->vopD3DContext->Flush( );

   this->mCreateDeviceResources( );
   this->vopD2DContext->SetDpi( this->vfDPI, this->vfDPI );
   this->mCreateWindowSizeDependentResources( );

   if( this->vopDeviceNotify != nullptr )
   {
      this->vopDeviceNotify->MOnDeviceRestored( );
   }
}

/**
 * @brief
 *
 *
 * @details
 * @par
 * This method registers the DeviceNotify to be informed on devie lost and creation.
 */
void DX::DeviceResources::MRegisterDeviceNotify( IDeviceNotify* aopDeviceNotify )
{
   this->vopDeviceNotify = aopDeviceNotify;
}

/**
 * @brief
 * 
 *
 * @details
 * @par
 * This method provides a hint to the driver that the application is entering an idle state and that temporary buffers
 * can be reclaimed for use by other applications. This method should be called when the application suspends.
 */
void DX::DeviceResources::MTrim( void )
{
   ComPtr< IDXGIDevice3 > koDXGIDevice;
   this->vopD3DDevice.As( &koDXGIDevice );
   koDXGIDevice->Trim( );
}

/**
 * @brief
 *
 * @details
 * @par
 * This method presents the contents of the swap chain to the screen. 
 */
void DX::DeviceResources::MPresent( void )
{
   HRESULT kuiHR;

   /// The first argument instructs DXGI to block until VSync, putting the application to sleep until the next VSync.
   /// This ensures we don't waste any cycles rendering frames that will never be displayed to the screen.
   kuiHR = this->vopSwapChain->Present( 1, 0 );

   /// -# Discard the contents of the render target.
   ///    This is a valid operation only when the existing contents will be entirely overwritten. If dirty or scroll
   ///    rects are used, this call should be removed.
   this->vopD3DContext->DiscardView( this->vopD3DRenderTargetView.Get( ) );

   /// -# Discard the contents of the depth stencil
   this->vopD3DContext->DiscardView( this->vopD3DDepthStencilView.Get( ) );

   /// -# If the device was removed either by a disconnection or a driver upgrade, must recreate all device resources
   if( ( kuiHR == DXGI_ERROR_DEVICE_REMOVED ) || ( kuiHR == DXGI_ERROR_DEVICE_RESET ) )
   {
      this->MHandleDeviceLost( );
   }
   else
   {
      DX::ThrowIfFailed( kuiHR );
   }
}

DX::DeviceResources::DeviceResources( const DeviceResources& aorResources )
{
   /// -# Call the assignment operator
   *this = aorResources;
}

DX::DeviceResources& DX::DeviceResources::operator=( const DeviceResources& aorResources )
{
   if( this != &aorResources )
   {
      /// TODO
   }

   return( *this );
}

/**
 * @brief
 *
 * @etails
 * @par
 * This method configures resources that do not depend on the Direct3D device.
 */
void DX::DeviceResources::mCreateDeviceIndependentResources( void )
{
   /// -# Initialize Direct2D resources
   D2D1_FACTORY_OPTIONS koOptions;

   ZeroMemory( &koOptions, sizeof( D2D1_FACTORY_OPTIONS ) );

#if defined( _DEBUG )
   koOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

   /// -# Initialize the Direct2D Factory
   DX::ThrowIfFailed( D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, 
                      __uuidof( ID2D1Factory2 ), &koOptions, &this->vopD2DFactory ) );

   /// -# Initialize the DirectWrite Factory
   DX::ThrowIfFailed( DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, 
                      __uuidof( IDWriteFactory2 ), &this->vopDWriteFactory ) );

   /// -# Initialize the Windows Imaging Component (WIC) Factory
   DX::ThrowIfFailed( CoCreateInstance( CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, 
                                        IID_PPV_ARGS( &this->vopWICFactory ) ) );
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
void DX::DeviceResources::mCreateDeviceResources( void )
{
   // This flag adds support for surfaces with a different color channel ordering
   // than the API default. It is required for compatibility with Direct2D.
   UINT kuiCreationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

   // This array defines the set of DirectX hardware feature levels this app will support.
   // Note the ordering should be preserved.
   // Don't forget to declare your application's minimum required feature level in its
   // description.  All applications are assumed to support 9.1 unless otherwise stated.
   D3D_FEATURE_LEVEL kepFeatureLevels[ ] =
   {
       D3D_FEATURE_LEVEL_11_1,
       D3D_FEATURE_LEVEL_11_0,
       D3D_FEATURE_LEVEL_10_1,
       D3D_FEATURE_LEVEL_10_0,
       D3D_FEATURE_LEVEL_9_3,
       D3D_FEATURE_LEVEL_9_2,
       D3D_FEATURE_LEVEL_9_1
   };

   // Create the Direct3D 11 API device object and a corresponding context.
   ComPtr< ID3D11Device >        kopDevice;
   ComPtr< ID3D11DeviceContext > kopContext;
   ComPtr< IDXGIDevice3 >        kopDXGIDevice;
   HRESULT                       kiHR;

#if defined(_DEBUG)
   if( DX::SdkLayersAvailable( ) )
   {
      // If the project is in a debug build, enable debugging via SDK Layers with this flag.
      kuiCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;
   }
#endif

   kiHR = D3D11CreateDevice( nullptr,                       // Specify nullptr to use the default adapter.
                             D3D_DRIVER_TYPE_HARDWARE,      // Create a device using the hardware graphics driver.
                             0,                             // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
                             kuiCreationFlags,              // Set debug and Direct2D compatibility flags.
                             kepFeatureLevels,              // List of feature levels this app can support.
                             ARRAYSIZE( kepFeatureLevels ), // Size of the list above.
                             D3D11_SDK_VERSION,             // Always set this to D3D11_SDK_VERSION for Windows Runtime apps.
                             &kopDevice,                    // Returns the Direct3D device created.
                             &this->veD3DFeatureLevel,      // Returns feature level of device created.
                             &kopContext                    // Returns the device immediate context.
   );

   if( FAILED( kiHR ) )
   {
      // If the initialization fails, fall back to the WARP device.
      // For more information on WARP, see: 
      // http://go.microsoft.com/fwlink/?LinkId=286690
      DX::ThrowIfFailed( D3D11CreateDevice( nullptr,
                                            D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
                                            0,
                                            kuiCreationFlags,
                                            kepFeatureLevels,
                                            ARRAYSIZE( kepFeatureLevels ),
                                            D3D11_SDK_VERSION,
                                            &kopDevice,
                                            &this->veD3DFeatureLevel,
                                            &kopContext )
      );
   }

   // Store pointers to the Direct3D 11.1 API device and immediate context.
   DX::ThrowIfFailed( kopDevice.As( &this->vopD3DDevice ) );
   DX::ThrowIfFailed( kopContext.As( &this->vopD3DContext ) );

   // Create the Direct2D device object and a corresponding context.
   DX::ThrowIfFailed( this->vopD3DDevice.As( &kopDXGIDevice ) );
   DX::ThrowIfFailed( this->vopD2DFactory->CreateDevice( kopDXGIDevice.Get( ), &this->vopD2DDevice ) );
   DX::ThrowIfFailed( this->vopD2DDevice->CreateDeviceContext( D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &this->vopD2DContext ) );
}

void DX::DeviceResources::mCreateWindowSizeDependentResources( void )
{
   // Clear the previous window size specific context.
   ID3D11RenderTargetView*        kopNullViews[ ] = { nullptr };
   DXGI_MODE_ROTATION             keDisplayRotation;
   bool                           kbSwapDimensions;
   HRESULT                        kiHR;
   DXGI_SWAP_CHAIN_DESC1          koSwapChainDesc = { 0 };
   ComPtr< IDXGIDevice3    >      kopDXGIDevice;
   ComPtr< IDXGIAdapter    >      kopDXGIAdapter;
   ComPtr< IDXGIFactory3   >      kopDXGIFactory;
   ComPtr< ID3D11Texture2D >      kopBackBuffer;
   ComPtr< ID3D11Texture2D >      kopDepthStencil;
   ComPtr< IDXGIResource1  >      kopDXGIBackBuffer;
   ComPtr< IDXGISurface2   >      kopDXGISurface;
   CD3D11_DEPTH_STENCIL_VIEW_DESC koDepthStencilViewDesc( D3D11_DSV_DIMENSION_TEXTURE2D );
   D2D1_BITMAP_PROPERTIES1        koBitmapProperties;

   this->vopD3DContext->OMSetRenderTargets( ARRAYSIZE( kopNullViews ), kopNullViews, nullptr );
   this->vopD3DRenderTargetView = nullptr;
   this->vopD3DRenderTargetViewRight = nullptr;
   this->vopD2DContext->SetTarget( nullptr );
   this->vopD2DTargetBitmap = nullptr;
   this->vopD2DTargetBitmapRight = nullptr;
   this->vopD3DDepthStencilView = nullptr;
   this->vopD3DContext->Flush( );

   // Calculate the necessary swap chain and render target size in pixels.
   this->voOutputSize.Width  = DX::ConvertDipsToPixels( this->voLogicalSize.Width,  this->vfDPI );
   this->voOutputSize.Height = DX::ConvertDipsToPixels( this->voLogicalSize.Height, this->vfDPI );

   // Prevent zero size DirectX content from being created.
   this->voOutputSize.Width  = max( this->voOutputSize.Width,  1 );
   this->voOutputSize.Height = max( this->voOutputSize.Height, 1 );

   // The width and height of the swap chain must be based on the window's
   // natively-oriented width and height. If the window is not in the native
   // orientation, the dimensions must be reversed.
   keDisplayRotation = this->mComputeDisplayRotation( );

   kbSwapDimensions = ( keDisplayRotation == DXGI_MODE_ROTATION_ROTATE90  ) || 
                      ( keDisplayRotation == DXGI_MODE_ROTATION_ROTATE270 );
   this->voD3DRenderTargetSize.Width  = kbSwapDimensions ? this->voOutputSize.Height : this->voOutputSize.Width;
   this->voD3DRenderTargetSize.Height = kbSwapDimensions ? this->voOutputSize.Width  : this->voOutputSize.Height;

   if( this->vopSwapChain != nullptr )
   {
      // If the swap chain already exists, resize it.
      kiHR = this->vopSwapChain->ResizeBuffers( 2, // Double-buffered swap chain.
                                                static_cast< UINT >( this->voD3DRenderTargetSize.Width ),
                                                static_cast< UINT >( this->voD3DRenderTargetSize.Height ),
                                                DXGI_FORMAT_B8G8R8A8_UNORM,
                                                0 );

      if( ( kiHR == DXGI_ERROR_DEVICE_REMOVED ) || ( kiHR == DXGI_ERROR_DEVICE_RESET ) )
      {
         // If the device was removed for any reason, a new device and swap chain will need to be created.
         this->MHandleDeviceLost( );

         // Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method 
         // and correctly set up the new device.
         return;
      }
      else
      {
         DX::ThrowIfFailed( kiHR );
      }
   }
   else
   {
      this->mCheckStereoEnabledStatus( );

      // Otherwise, create a new one using the same adapter as the existing Direct3D device.
      koSwapChainDesc.Width  = static_cast< UINT >( this->voD3DRenderTargetSize.Width ); // Match the size of the window.
      koSwapChainDesc.Height = static_cast< UINT >( this->voD3DRenderTargetSize.Height );
      koSwapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swap chain format.
      koSwapChainDesc.Stereo = this->vbStereoEnabled;
      koSwapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
      koSwapChainDesc.SampleDesc.Quality = 0;
      koSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      koSwapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
      koSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
      koSwapChainDesc.Flags = 0;
      koSwapChainDesc.Scaling = DXGI_SCALING_NONE;
      koSwapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

      // This sequence obtains the DXGI factory that was used to create the Direct3D device above.
      DX::ThrowIfFailed( this->vopD3DDevice.As( &kopDXGIDevice ) );
      DX::ThrowIfFailed( kopDXGIDevice->GetAdapter( &kopDXGIAdapter ) );
      DX::ThrowIfFailed( kopDXGIAdapter->GetParent( IID_PPV_ARGS( &kopDXGIFactory ) ) );
      DX::ThrowIfFailed( kopDXGIFactory->CreateSwapChainForCoreWindow( this->vopD3DDevice.Get( ),
                                                                       reinterpret_cast< IUnknown* >( this->vopWindow.Get( ) ),
                                                                       &koSwapChainDesc,
                                                                       nullptr,
                                                                       &this->vopSwapChain ) );

      // Ensure that DXGI does not queue more than one frame at a time. This both reduces latency and
      // ensures that the application will only render after each VSync, minimizing power consumption.
      DX::ThrowIfFailed( kopDXGIDevice->SetMaximumFrameLatency( 1 ) );
   }

   // Set the proper orientation for the swap chain, and generate 2D and
   // 3D matrix transformations for rendering to the rotated swap chain.
   // Note the rotation angle for the 2D and 3D transforms are different.
   // This is due to the difference in coordinate spaces.  Additionally,
   // the 3D matrix is specified explicitly to avoid rounding errors.
   switch( keDisplayRotation )
   {
      case DXGI_MODE_ROTATION_IDENTITY:
      {
         this->voOrientationTransform2D = Matrix3x2F::Identity( );
         this->voOrientationTransform3D = ScreenRotation::Rotation0;
         break;
      }
      case DXGI_MODE_ROTATION_ROTATE90:
      {
         this->voOrientationTransform2D = Matrix3x2F::Rotation( 90.0f ) * 
                                          Matrix3x2F::Translation( this->voLogicalSize.Height, 0.0f );
         this->voOrientationTransform3D = ScreenRotation::Rotation270;
         break;
      }
      case DXGI_MODE_ROTATION_ROTATE180:
      {
         this->voOrientationTransform2D = Matrix3x2F::Rotation( 180.0f ) *
                                          Matrix3x2F::Translation( this->voLogicalSize.Width, this->voLogicalSize.Height );
         this->voOrientationTransform3D = ScreenRotation::Rotation180;
         break;
      }
      case DXGI_MODE_ROTATION_ROTATE270:
      {
         this->voOrientationTransform2D = Matrix3x2F::Rotation( 270.0f ) *
                                          Matrix3x2F::Translation( 0.0f, this->voLogicalSize.Width );
         this->voOrientationTransform3D = ScreenRotation::Rotation90;
         break;
      }
      default:
      {
         throw ref new FailureException( );
         break;
      }
   }

   DX::ThrowIfFailed( this->vopSwapChain->SetRotation( keDisplayRotation ) );

   // Create a render target view of the swap chain back buffer.
   DX::ThrowIfFailed( this->vopSwapChain->GetBuffer( 0, IID_PPV_ARGS( &kopBackBuffer ) ) );
   DX::ThrowIfFailed( this->vopD3DDevice->CreateRenderTargetView( kopBackBuffer.Get( ), 
                                                                  nullptr, 
                                                                  &this->vopD3DRenderTargetView ) );

   // Stereo swapchains have an arrayed resource, so create a second Render Target
   // for the right eye buffer.
   if( this->vbStereoEnabled )
   {
      CD3D11_RENDER_TARGET_VIEW_DESC koRenderTargetViewRightDesc( D3D11_RTV_DIMENSION_TEXTURE2DARRAY, 
                                                                  DXGI_FORMAT_B8G8R8A8_UNORM, 0, 1, 1 );

      DX::ThrowIfFailed( this->vopD3DDevice->CreateRenderTargetView( kopBackBuffer.Get( ), 
                                                                     &koRenderTargetViewRightDesc,
                                                                     &this->vopD3DRenderTargetViewRight ) );
   }

   // Create a depth stencil view for use with 3D rendering if needed.
   CD3D11_TEXTURE2D_DESC koDepthStencilDesc( DXGI_FORMAT_D24_UNORM_S8_UINT,
                                             static_cast< UINT >( this->voD3DRenderTargetSize.Width ),
                                             static_cast< UINT >( this->voD3DRenderTargetSize.Height ),
                                             1, // This depth stencil view has only one texture.
                                             1, // Use a single mipmap level.
                                             D3D11_BIND_DEPTH_STENCIL );

   DX::ThrowIfFailed( this->vopD3DDevice->CreateTexture2D( &koDepthStencilDesc, nullptr, &kopDepthStencil ) );
   DX::ThrowIfFailed( this->vopD3DDevice->CreateDepthStencilView( kopDepthStencil.Get( ), &koDepthStencilViewDesc,
                                                                  &this->vopD3DDepthStencilView ) );

   // Set the 3D rendering viewport to target the entire window.
   this->voScreenViewport = CD3D11_VIEWPORT( 0.0f, 0.0f, this->voD3DRenderTargetSize.Width, this->voD3DRenderTargetSize.Height );
   this->vopD3DContext->RSSetViewports( 1, &this->voScreenViewport );

   // Create a Direct2D target bitmap associated with the
   // swap chain back buffer and set it as the current target.
   koBitmapProperties = D2D1::BitmapProperties1( D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                                                 D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED ),
                                                 this->vfDPI, this->vfDPI );

   DX::ThrowIfFailed( this->vopSwapChain->GetBuffer( 0, IID_PPV_ARGS( &kopDXGIBackBuffer ) ) );
   DX::ThrowIfFailed( kopDXGIBackBuffer->CreateSubresourceSurface( 0, &kopDXGISurface ) );
   DX::ThrowIfFailed( this->vopD2DContext->CreateBitmapFromDxgiSurface( kopDXGISurface.Get( ), &koBitmapProperties,
                                                                        &this->vopD2DTargetBitmap ) );

   // Stereo swapchains have an arrayed resource, so create a second Target Bitmap
   // for the right eye buffer.
   if( this->vbStereoEnabled )
   {
      DX::ThrowIfFailed( kopDXGIBackBuffer->CreateSubresourceSurface( 1, &kopDXGISurface ) );
      DX::ThrowIfFailed( this->vopD2DContext->CreateBitmapFromDxgiSurface( kopDXGISurface.Get( ),
                                                                           &koBitmapProperties,
                                                                           &this->vopD2DTargetBitmapRight ) );
   }

   this->vopD2DContext->SetTarget( this->vopD2DTargetBitmap.Get( ) );

   // Grayscale text anti-aliasing is recommended for all Windows Store apps.
   this->vopD2DContext->SetTextAntialiasMode( D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE );
}

/**
 * @brief
 *
 *
 * @details
 * @par
 * This method determines the rotation between the display device's native orientation and the current display orientation.
 */
DXGI_MODE_ROTATION DX::DeviceResources::mComputeDisplayRotation( void )
{
   DXGI_MODE_ROTATION keRotation = DXGI_MODE_ROTATION_UNSPECIFIED;

   switch( this->veNativeOrientation )
   {
      case DisplayOrientations::Landscape:
      {
         switch( this->veCurrentOrientation )
         {
            case DisplayOrientations::Landscape:
            {
               keRotation = DXGI_MODE_ROTATION_IDENTITY;
               break;
            }
            case DisplayOrientations::Portrait:
            {
               keRotation = DXGI_MODE_ROTATION_ROTATE270;
               break;
            }
            case DisplayOrientations::LandscapeFlipped:
            {
               keRotation = DXGI_MODE_ROTATION_ROTATE180;
               break;
            }
            case DisplayOrientations::PortraitFlipped:
            {
               keRotation = DXGI_MODE_ROTATION_ROTATE90;
               break;
            }
         }
      }
      case DisplayOrientations::Portrait:
      {
         switch( this->veCurrentOrientation )
         {
            case DisplayOrientations::Landscape:
            {
               keRotation = DXGI_MODE_ROTATION_ROTATE90;
               break;
            }
            case DisplayOrientations::Portrait:
            {
               keRotation = DXGI_MODE_ROTATION_IDENTITY;
               break;
            }
            case DisplayOrientations::LandscapeFlipped:
            {
               keRotation = DXGI_MODE_ROTATION_ROTATE270;
               break;
            }
            case DisplayOrientations::PortraitFlipped:
            {
               keRotation = DXGI_MODE_ROTATION_ROTATE180;
               break;
            }
         }
      }
   }

   return( keRotation );
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
void DX::DeviceResources::mCheckStereoEnabledStatus( void )
{
   ComPtr< IDXGIDevice1  > koDXGIDevice;
   ComPtr< IDXGIAdapter  > koDXGIAdapter;
   ComPtr< IDXGIFactory2 > koDXGIFactory;

   /// -# Retrieve the underlying DXGI Device from the D3D Device
   DX::ThrowIfFailed( this->vopD3DDevice.As( &koDXGIDevice ) );

   /// -# Get the associated adapter from the DXGI Device
   DX::ThrowIfFailed( koDXGIDevice->GetAdapter( &koDXGIAdapter ) );

   /// -# Get the parent factory from the DXGI Adapter
   DX::ThrowIfFailed( koDXGIAdapter->GetParent( IID_PPV_ARGS( &koDXGIFactory ) ) );

   this->vbStereoEnabled = koDXGIFactory->IsWindowedStereoEnabled( ) ? true : false;
}

