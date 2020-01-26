/**
 * @file
 * @brief
 *
 *
 * @details
 * @par
 *
 */
#pragma once

namespace Example
{
   namespace NResources
   {
      namespace NRendering
      {
         ref class TcMaterial
         {
         private:       // Private Attributes
            DirectX::XMFLOAT4 voMeshColor;
            DirectX::XMFLOAT4 voDiffuseColor;
            DirectX::XMFLOAT4 voHitColor;
            DirectX::XMFLOAT4 voSpecularColor;
            float             vfSpecularExponent;

            Microsoft::WRL::ComPtr< ID3D11VertexShader >       vopVertexShader;
            Microsoft::WRL::ComPtr< ID3D11PixelShader >        vopPixelShader;
            Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > vopTextureRV;

         internal:      // Public Methods Accessible To Assembly
            TcMaterial( DirectX::XMFLOAT4 aoMeshColor, DirectX::XMFLOAT4 aoDiffuseColor, DirectX::XMFLOAT4 aoSpecularColor,
                        float afSpecularExponent, _In_ ID3D11ShaderResourceView& aorTextureResourceView, 
                        _In_ ID3D11VertexShader& aorVertexShader, _In_ ID3D11PixelShader& aorPixelShader );

            void MRenderSetup( _In_ ID3D11DeviceContext& aorContext, 
                               _Inout_ TsConstantBufferChangesEveryPrim& aorConstantBuffer );

            void MSetTexture( _In_ ID3D11ShaderResourceView& aorTextureResourceView );
         };
      }
   }
}
