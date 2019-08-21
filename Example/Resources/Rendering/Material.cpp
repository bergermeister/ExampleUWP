/**
 * @file
 * @brief
 *
 *
 * @details
 * @par
 *
 */
#include <pch.h>
#include <Resources/Rendering/Material.h>
#include <Constants.h>

using namespace Example::NResources::NRendering;

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
TcMaterial::TcMaterial( DirectX::XMFLOAT4 aoMeshColor, DirectX::XMFLOAT4 aoDiffuseColor, DirectX::XMFLOAT4 aoSpecularColor,
                       float afSpecularExponent, _In_ ID3D11ShaderResourceView& aorTextureResourceView,
                       _In_ ID3D11VertexShader& aorVertexShader, _In_ ID3D11PixelShader& aorPixelShader )
{
   this->voMeshColor        = aoMeshColor;
   this->voDiffuseColor     = aoDiffuseColor;
   this->voSpecularColor    = aoSpecularColor;
   this->vfSpecularExponent = afSpecularExponent;
   this->vopVertexShader    = &aorVertexShader;
   this->vopPixelShader     = &aorPixelShader;
   this->vopTextureRV       = &aorTextureResourceView;
}

/**
 * @brief
 * 
 *
 * @details
 * @par
 *
 */
void TcMaterial::MRenderSetup( _In_ ID3D11DeviceContext& aorContext,
                               _Inout_ TsConstantBufferChangesEveryPrim& aorConstantBuffer )
{
   aorConstantBuffer.VoMeshColor = this->voMeshColor;
   aorConstantBuffer.VoSpecularColor = this->voSpecularColor;
   aorConstantBuffer.VfSpecularPower = this->vfSpecularExponent;
   aorConstantBuffer.VoDiffuseColor  = this->voDiffuseColor;

   aorContext.PSSetShaderResources( 0, 1, this->vopTextureRV.GetAddressOf( ) );
   aorContext.VSSetShader( this->vopVertexShader.Get( ), nullptr, 0 );
   aorContext.PSSetShader( this->vopPixelShader.Get( ), nullptr, 0 );
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
void TcMaterial::MSetTexture( _In_ ID3D11ShaderResourceView& aorTextureResourceView )
{
   this->vopTextureRV = &aorTextureResourceView;
}
