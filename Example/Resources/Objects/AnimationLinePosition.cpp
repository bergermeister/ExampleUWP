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
#include <Resources/Objects/Animation.h>
#include <Resources/Objects/AnimationLinePosition.h>
#include <Common/DirectXHelper.h>

using namespace Example::NResources::NObjects;
using namespace DirectX;

TcAnimationLinePosition::TcAnimationLinePosition( _In_ DirectX::XMFLOAT3 aoStartPosition,
                                                  _In_ DirectX::XMFLOAT3 aoEndPosition,
                                                  _In_ float             afDuration,
                                                  _In_ bool              abContinuous )
{
   this->voStartPosition = aoStartPosition;
   this->voEndPosition   = aoEndPosition;
   this->vfDuration      = afDuration;
   this->vbContinuous    = abContinuous;
   this->vfLength        = XMVectorGetX( XMVector3Length( XMLoadFloat3( &aoEndPosition ) - 
                                                          XMLoadFloat3( &aoStartPosition ) ) );
}


XMFLOAT3 TcAnimationLinePosition::MEvaluate( _In_ float afT )
{
   XMFLOAT3 koPosition;
   float    kfStartTime = this->vfStartTime;
   float    kfDelta;

   if( afT <= this->vfStartTime )
   {
      koPosition = this->voStartPosition;
   }
   else if( ( afT >= ( this->vfStartTime + this->vfDuration ) ) && !this->vbContinuous )
   {
      koPosition = this->voEndPosition;
   }
   else
   {
      if( this->vbContinuous )
      {
         kfStartTime += ( float )( ( int )( ( afT - this->vfStartTime ) / this->vfDuration ) ) * this->vfDuration;
      }

      kfDelta = ( afT - kfStartTime ) / this->vfDuration;
      koPosition.x = this->voStartPosition.x + ( ( this->voEndPosition.x - this->voStartPosition.x ) * kfDelta );
      koPosition.y = this->voStartPosition.y + ( ( this->voEndPosition.y - this->voStartPosition.y ) * kfDelta );
      koPosition.z = this->voStartPosition.z + ( ( this->voEndPosition.z - this->voStartPosition.z ) * kfDelta );
   }

   return( koPosition );
}

