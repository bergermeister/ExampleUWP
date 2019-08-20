#pragma once

namespace DX
{
   static const UINT c_frameCount = 3;      // Use triple buffering.

   class IDeviceNotify
   {
   public:
      virtual void MOnDeviceLost( ) = 0;
      virtual void MOnDeviceRestored( ) = 0;
   };

   /**
    * @brief
    *
    * @details
    * @par
    * Controls all the DirectX device resources.
    */
   class DeviceResources
   {
   public:        // Public Attributes
      // Device Accessors
      Windows::Foundation::Size MOutputSize( void ) const       { return( this->voOutputSize          ); }
      Windows::Foundation::Size MLogicalSize( void ) const      { return( this->voLogicalSize         ); }
      Windows::Foundation::Size MRenderTargetSize( void ) const { return( this->voD3DRenderTargetSize ); }
      bool                      MStereoEnabled( void ) const    { return( this->vbStereoEnabled       ); }
      float                     MDPI( void ) const              { return( this->vfDPI                 ); }

      // D3D Accessors.
      ID3D11Device3*          MD3DDevice( void ) const                       { return( this->vopD3DDevice.Get( )                 ); }
      ID3D11DeviceContext3*   MD3DDeviceContext( void ) const                { return( this->vopD3DContext.Get( )               ); }
      IDXGISwapChain1*        MSwapChain( void ) const                       { return( this->vopSwapChain.Get( )                ); }
      D3D_FEATURE_LEVEL       MDeviceFeatureLevel( void ) const              { return( this->veD3DFeatureLevel                  ); }
      ID3D11RenderTargetView* MBackBufferRenderTargetView( void ) const      { return( this->vopD3DRenderTargetView.Get( )      ); }
      ID3D11RenderTargetView* MBackBufferRenderTargetViewRight( void ) const { return( this->vopD3DRenderTargetViewRight.Get( ) ); }
      ID3D11DepthStencilView* MDepthStencilView( void ) const                { return( this->vopD3DDepthStencilView.Get( ) ); }
      D3D11_VIEWPORT          MScreenViewport( void ) const                  { return( this->voScreenViewport                   ); }
      DirectX::XMFLOAT4X4     MOrientationTransform3D( void ) const          { return( this->voOrientationTransform3D           ); }

      // D2D Accessors
      ID2D1Factory3*       MD2DFactory( void ) const             { return( this->vopD2DFactory.Get( ) ); }
      ID2D1Device2*        MD2DDevice( void ) const              { return( this->vopD2DDevice.Get( ) ); }
      ID2D1DeviceContext2* MD2DDeviceContext( void ) const       { return( this->vopD2DContext.Get( ) ); }
      ID2D1Bitmap1*        MD2DTargetBitmap( void ) const        { return( this->vopD2DTargetBitmap.Get( ) ); }
      ID2D1Bitmap1*        MD2DTargetBitmapRight( void ) const   { return( this->vopD2DTargetBitmapRight.Get( ) ); }
      IDWriteFactory3*     MDWriteFactory( void ) const          { return( this->vopDWriteFactory.Get( ) ); }
      IWICImagingFactory2* MWICImagingFactory( void ) const      { return( this->vopWICFactory.Get( ) ); }
      D2D1::Matrix3x2F     MOrientationTransform2D( void ) const { return( this->voOrientationTransform2D ); }

   private:       // Private Attributes
      // Direct3D objects.
      Microsoft::WRL::ComPtr< ID3D11Device3 >        vopD3DDevice;
      Microsoft::WRL::ComPtr< ID3D11DeviceContext3 > vopD3DContext;
      Microsoft::WRL::ComPtr< IDXGISwapChain1 >      vopSwapChain;

      // Direct3D Rendering Objects (Required for 3D)
      Microsoft::WRL::ComPtr< ID3D11RenderTargetView > vopD3DRenderTargetView;
      Microsoft::WRL::ComPtr< ID3D11RenderTargetView > vopD3DRenderTargetViewRight;
      Microsoft::WRL::ComPtr< ID3D11DepthStencilView > vopD3DDepthStencilView;
      D3D11_VIEWPORT                                   voScreenViewport;

      // Direct2D Drawing Components
      Microsoft::WRL::ComPtr< ID2D1Factory3 >       vopD2DFactory;
      Microsoft::WRL::ComPtr< ID2D1Device2 >        vopD2DDevice;
      Microsoft::WRL::ComPtr< ID2D1DeviceContext2 > vopD2DContext;
      Microsoft::WRL::ComPtr< ID2D1Bitmap1 >        vopD2DTargetBitmap;
      Microsoft::WRL::ComPtr< ID2D1Bitmap1 >        vopD2DTargetBitmapRight;

      // DirectWrite Drawing Components
      Microsoft::WRL::ComPtr< IDWriteFactory3 >     vopDWriteFactory;
      Microsoft::WRL::ComPtr< IWICImagingFactory2 > vopWICFactory;

      // Cached Reference to the Window
      Platform::Agile< Windows::UI::Core::CoreWindow > vopWindow;

      // Cached Device Properties
      D3D_FEATURE_LEVEL                               veD3DFeatureLevel;
      Windows::Foundation::Size                       voD3DRenderTargetSize;
      Windows::Foundation::Size                       voOutputSize;
      Windows::Foundation::Size                       voLogicalSize;
      Windows::Graphics::Display::DisplayOrientations veNativeOrientation;
      Windows::Graphics::Display::DisplayOrientations veCurrentOrientation;
      float                                           vfDPI;
      bool                                            vbStereoEnabled;

      // Transform used for display orientation
      D2D1::Matrix3x2F    voOrientationTransform2D;
      DirectX::XMFLOAT4X4 voOrientationTransform3D;

      // The IdeviceNotify can be held directly as it owns the DeviceResources
      IDeviceNotify* vopDeviceNotify;

   public:        // Public Methods
      DeviceResources( void );
      ~DeviceResources( void );

      void MSetWindow( Windows::UI::Core::CoreWindow^ aopWindow );
      void MSetLogicalSize( Windows::Foundation::Size aoLogicalSize );
      void MSetCurrentOrientation( Windows::Graphics::Display::DisplayOrientations aeCurrentOrientation );
      void MSetDPI( float afDPI );
      void MUpdateStereoState( void );
      void MValidateDevice( void );
      void MHandleDeviceLost( void );
      void MRegisterDeviceNotify( IDeviceNotify* aopDeviceNotify );
      void MTrim( void );
      void MPresent( void );

   private:       // Private Methods
      DeviceResources( const DeviceResources& aorResources );            // Disable
      DeviceResources& operator=( const DeviceResources& aorResources ); // Disable

      void               mCreateDeviceIndependentResources( void );
      void               mCreateDeviceResources( void );
      void               mCreateWindowSizeDependentResources( void );
      DXGI_MODE_ROTATION mComputeDisplayRotation( void );
      void               mCheckStereoEnabledStatus( void );
   };
}
