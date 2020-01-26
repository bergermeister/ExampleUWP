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
         ref class TcLineListPosition : public TcAnimation
         {
         private:       // Private Attributes
            unsigned int                 vuiCount;
            float                        vfTotalLength;
            std::vector< TsLineSegment > voSegment;

         internal:      // Public Methods Accessible to Assembly
            TcLineListPosition( _In_ unsigned int                        auiCount,
                                _In_reads_( auiCount ) DirectX::XMFLOAT3 aoPosition[ ], 
                                _In_ float                               afDuration,
                                _In_ bool                                abContinuous );
            virtual DirectX::XMFLOAT3 MEvaluate( _In_ float afT ) override;
         };
      }
   }
}

