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

using namespace Example::NResources::NAnimations;
using namespace DirectX;

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
TcLineListPosition::TcLineListPosition( _In_ unsigned int                        auiCount,
                                        _In_reads_( auiCount ) DirectX::XMFLOAT3 aoPosition[ ],
                                        _In_ float                               afDuration,
                                        _In_ bool                                abContinuous )
{
   std::vector< TsLineSegment > koSegment( auiCount );
   unsigned int                 kuiI;
   float                        kfU = 0.0f;

   this->vfDuration    = afDuration;
   this->vbContinuous  = abContinuous;
   this->vuiCount      = auiCount;
   this->voSegment     = koSegment;
   this->vfTotalLength = 0.0f;

   this->voSegment[ 0 ].voPosition = aoPosition[ 0 ];
   for( kuiI = 1; kuiI < auiCount; kuiI++ )
   {
      this->voSegment[ kuiI ].voPosition = aoPosition[ kuiI ];
      this->voSegment[ kuiI - 1 ].vfLength = 
         XMVectorGetX( XMVector3Length( XMLoadFloat3( &this->voSegment[ kuiI ].voPosition ) -
                                        XMLoadFloat3( &this->voSegment[ kuiI - 1 ].voPosition ) ) );
      this->vfTotalLength += this->voSegment[ kuiI - 1 ].vfLength;
   }

   // Parameterize the segments to ensure uniform evaluation along the path.
   for( kuiI = 0; kuiI < ( auiCount - 1 ); kuiI++ )
   {
      this->voSegment[ kuiI ].vfStart  = kfU;
      this->voSegment[ kuiI ].vfULength = ( this->voSegment[ kuiI ].vfLength / this->vfTotalLength );
      kfU += this->voSegment[ kuiI ].vfULength;
   }
   this->voSegment[ auiCount - 1 ].vfStart = 1.0f;
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
XMFLOAT3 TcLineListPosition::MEvaluate( _In_ float afT )
{
   XMFLOAT3     koPosition;
   float        kfU;
   float        kfStartTime;
   unsigned int kuiI;

   if( afT <= this->vfStartTime )
   {
      koPosition = this->voSegment[ 0 ].voPosition;
   }
   else if( ( afT >= ( this->vfStartTime + this->vfDuration ) ) && !this->vbContinuous )
   {
      koPosition = this->voSegment[ this->vuiCount - 1 ].voPosition;
   }
   else
   {
      kfStartTime = this->vfStartTime;

      if( this->vbContinuous )
      {
         // For continuous operation move the start time forward to eliminate previous iterations
         kfStartTime += ( float )( ( int )( ( afT - kfStartTime ) / this->vfDuration ) ) * this->vfDuration;
      }

      kfU = ( afT - this->vfStartTime ) / this->vfDuration;

      // Findd the right segment
      kuiI = 0;
      while( kfU > this->voSegment[ kuiI + 1 ].vfStart )
      {
         kuiI++;
      }

      kfU -= this->voSegment[ kuiI ].vfStart;
      kfU /= this->voSegment[ kuiI ].vfULength;

      koPosition.x = this->voSegment[ kuiI ].voPosition.x + ( ( this->voSegment[ kuiI + 1 ].voPosition.x - 
                                                                this->voSegment[ kuiI ].voPosition.x ) * kfU );
      koPosition.y = this->voSegment[ kuiI ].voPosition.y + ( ( this->voSegment[ kuiI + 1 ].voPosition.y - 
                                                                this->voSegment[ kuiI ].voPosition.y ) * kfU );
      koPosition.z = this->voSegment[ kuiI ].voPosition.z + ( ( this->voSegment[ kuiI + 1 ].voPosition.z - 
                                                                this->voSegment[ kuiI ].voPosition.z ) * kfU );
   }

   return( koPosition );
}

