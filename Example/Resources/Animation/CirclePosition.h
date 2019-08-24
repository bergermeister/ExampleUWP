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
      namespace NAnimations
      {
         ref class TcCirclePosition : public TcAnimation
         {
         private:       // Private Attributes
            DirectX::XMFLOAT4X4 voRotationMatrix;
            DirectX::XMFLOAT3   voCenter;
            DirectX::XMFLOAT3   voPlaneNormal;
            DirectX::XMFLOAT3   voStartPosition;
            float               vfRadius;
            bool                vbClockwise;

         internal:      // Public Methods Accessible to Assembly
            TcCirclePosition( _In_ DirectX::XMFLOAT3 aoCenter,
                              _In_ DirectX::XMFLOAT3 aoStartPosition,
                              _In_ DirectX::XMFLOAT3 aoPlaneNormal,
                              _In_ float             afDuration,
                              _In_ bool              abContinuous,
                              _In_ bool              abClockwise );
            virtual DirectX::XMFLOAT3 MEvaluate( _In_ float afT ) override;
         };
      }
   }
}

