#pragma once

namespace Example
{
   namespace NResources
   {
      namespace NRendering
      {
         struct TsPNTVertex
         {
            DirectX::XMFLOAT3 VoPosition;
            DirectX::XMFLOAT3 VoNormal;
            DirectX::XMFLOAT2 VoTextureCoordinate;
         };

         static D3D11_INPUT_ELEMENT_DESC TsPNTVertexLayout[ ] =
         {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
         };

         struct TsConstantBufferNeverChanges
         {
            DirectX::XMFLOAT4 VoLightPosition[ 4 ];
            DirectX::XMFLOAT4 VoLightColor;
         };

         struct TsConstantBufferChangeOnResize
         {
            DirectX::XMFLOAT4X4 VoProjection;
         };

         struct TsConstantBufferChangesEveryFrame
         {
            DirectX::XMFLOAT4X4 VoView;
         };

         struct TsConstantBufferChangesEveryPrim
         {
            DirectX::XMFLOAT4X4 VoWorldMatrix;
            DirectX::XMFLOAT4   VoMeshColor;
            DirectX::XMFLOAT4   VoDiffuseColor;
            DirectX::XMFLOAT4   VoSpecularColor;
            float               VfSpecularPower;
         };
      }
   }
}
