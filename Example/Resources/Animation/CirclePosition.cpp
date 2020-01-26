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
TcCirclePosition::TcCirclePosition( _In_ DirectX::XMFLOAT3 aoCenter,
                                    _In_ DirectX::XMFLOAT3 aoStartPosition,
                                    _In_ DirectX::XMFLOAT3 aoPlaneNormal,
                                    _In_ float             afDuration,
                                    _In_ bool              abContinuous,
                                    _In_ bool              abClockwise )
{
   XMVECTOR koCoordX;
   XMVECTOR koCoordZ;
   XMVECTOR koCoordY;
   XMVECTOR koVectorX;
   XMVECTOR koVectorY;
   XMMATRIX koMat1;
   XMMATRIX koMat2;
   XMVECTOR koAxis1;
   XMVECTOR koAxis2;

   this->voCenter        = aoCenter;
   this->voPlaneNormal   = aoPlaneNormal;
   this->voStartPosition = aoStartPosition;
   this->vfDuration      = afDuration;
   this->vbContinuous    = abContinuous;
   this->vbClockwise     = abClockwise;


   koCoordX = XMLoadFloat3( &this->voStartPosition ) - XMLoadFloat3( &this->voCenter );
   this->vfRadius = XMVectorGetX( XMVector3Length( koCoordX ) );

   XMVector3Normalize( koCoordX );

   koCoordZ = XMLoadFloat3( &this->voPlaneNormal );
   XMVector3Normalize( koCoordZ );

   if( this->vbClockwise )
   {
      koCoordY = XMVector3Cross( koCoordZ, koCoordX );
   }
   else
   {
      koCoordY = XMVector3Cross( koCoordX, koCoordZ );
   }

   koVectorX = XMVectorSet( 1.0f, 0.0f, 0.0f, 1.0f );
   koVectorY = XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f );
   koMat1 = XMMatrixIdentity( );
   koMat2 = XMMatrixIdentity( );

   if( !XMVector3Equal( koCoordX, koVectorX ) )
   {
      float angle;
      angle = XMVectorGetX(
         XMVector3AngleBetweenVectors( koVectorX, koCoordX )
      );
      if( ( angle * angle ) > 0.025 )
      {
         koAxis1 = XMVector3Cross( koVectorX, koCoordX );

         koMat1 = XMMatrixRotationAxis( koAxis1, angle );
         koVectorY = XMVector3TransformCoord( koVectorY, koMat1 );
      }
   }
   if( !XMVector3Equal( koVectorY, koCoordY ) )
   {
      float angle;
      angle = XMVectorGetX(
         XMVector3AngleBetweenVectors( koVectorY, koCoordY )
      );
      if( ( angle * angle ) > 0.025 )
      {
         koAxis2 = XMVector3Cross( koVectorY, koCoordY );
         koMat2 = XMMatrixRotationAxis( koAxis2, angle );
      }
   }
   XMStoreFloat4x4( &this->voRotationMatrix, koMat1 * koMat2 * 
                    XMMatrixTranslation( this->voCenter.x, this->voCenter.y,this->voCenter.z ) );
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
DirectX::XMFLOAT3 TcCirclePosition::MEvaluate( _In_ float afT )
{
   XMFLOAT3 koPosition;
   float    kfStartTime;
   float    kfU;

   if( afT <= this->vfStartTime )
   {
      koPosition = this->voStartPosition;
   }
   else if( ( afT >= ( this->vfStartTime + this->vfDuration ) ) && !this->vbContinuous )
   {
      koPosition = this->voStartPosition;
   }
   else
   {
      kfStartTime = this->vfStartTime;

      if( this->vbContinuous )
      {
         kfStartTime += (float )( (int )( ( afT - this->vfStartTime ) / this->vfDuration ) ) * this->vfDuration;
      }

      kfU = ( ( afT - this->vfStartTime ) / this->vfDuration ) * XM_2PI;

      koPosition.x = this->vfRadius * static_cast< float >( cos( kfU ) );
      koPosition.y = this->vfRadius * static_cast< float >( sin( kfU ) );
      koPosition.z = 0.0f;

      XMStoreFloat3( &koPosition, XMVector3TransformCoord( XMLoadFloat3( &koPosition ), 
                                                           XMLoadFloat4x4( &this->voRotationMatrix ) ) );
   }

   return( koPosition );
}
