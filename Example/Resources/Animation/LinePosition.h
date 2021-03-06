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
         ref class TcLinePosition : public TcAnimation
         {
         private:       // Private Attributes
            DirectX::XMFLOAT3 voStartPosition;
            DirectX::XMFLOAT3 voEndPosition;
            float             vfLength;

         internal:      // Public Methods Accessible to Assembly
            TcLinePosition( _In_ DirectX::XMFLOAT3 aoStartPosition,
                            _In_ DirectX::XMFLOAT3 aoEndPosition,
                            _In_ float             afDuration,
                            _In_ bool              abContinuous );
            virtual DirectX::XMFLOAT3 MEvaluate( _In_ float afT ) override;
         };
      }
   }
}

