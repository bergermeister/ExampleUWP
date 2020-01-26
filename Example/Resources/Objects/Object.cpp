/**
 * @file
 * @brief
 *
 * @details
 * @par
 *
 */
#include <pch.h>

using namespace DirectX;
using namespace Example::NResources::NObjects;

//--------------------------------------------------------------------------------

TcObject::TcObject( ) :
   m_normalMaterial( nullptr ),
   m_hitMaterial( nullptr )
{
   m_active          = false;
   m_target          = false;
   m_targetId        = 0;
   m_hit             = false;
   m_ground          = true;

   m_position        = XMFLOAT3( 0.0f, 0.0f, 0.0f );
   m_velocity        = XMFLOAT3( 0.0f, 0.0f, 0.0f );
   m_defaultXAxis    = XMFLOAT3( 1.0f, 0.0f, 0.0f );
   m_defaultYAxis    = XMFLOAT3( 0.0f, 1.0f, 0.0f );
   m_defaultZAxis    = XMFLOAT3( 0.0f, 0.0f, 1.0f );
   XMStoreFloat4x4( &m_modelMatrix, XMMatrixIdentity( ) );

   m_hitTime         = 0.0f;

   m_animatePosition = nullptr;
}

bool TcObject::MIsTouching( DirectX::XMFLOAT3        aoPoint,
                            float                    afRadius,
                            _Out_ DirectX::XMFLOAT3& aorContact,
                            _Out_ DirectX::XMFLOAT3& aorNormal )
{
   ( void )aoPoint;
   ( void )afRadius;
   aorContact = DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
   aorNormal  = DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f );
   return( false );
};

//----------------------------------------------------------------------

void TcObject::Render( _In_ ID3D11DeviceContext& aorContext, _In_ ID3D11Buffer& aorPrimitiveConstantBuffer )
{
   if( !m_active || ( m_mesh == nullptr ) || ( m_normalMaterial == nullptr ) )
   {
      return;
   }

   ConstantBufferChangesEveryPrim koConstantBuffer;

   XMStoreFloat4x4( &koConstantBuffer.worldMatrix, XMMatrixTranspose( ModelMatrix( ) )
   );

   if( m_hit && m_hitMaterial != nullptr )
   {
      m_hitMaterial->RenderSetup( context, &constantBuffer );
   }
   else
   {
      m_normalMaterial->RenderSetup( context, &constantBuffer );
   }
   context->UpdateSubresource( primitiveConstantBuffer, 0, nullptr, &constantBuffer, 0, 0 );

   m_mesh->Render( context );
}

//----------------------------------------------------------------------

void TcObject::PlaySound( float impactSpeed, XMFLOAT3 eyePoint )
{
   if( m_hitSound != nullptr )
   {
      // Determine the sound volume adjustment based on velocity.
      float adjustment;
      if( impactSpeed < GameConstants::Sound::MinVelocity )
      {
         adjustment = 0.0f;  // Too soft.  Don't play sound.
      }
      else
      {
         adjustment = min( 1.0f, impactSpeed / GameConstants::Sound::MaxVelocity );
         adjustment = GameConstants::Sound::MinAdjustment + adjustment * ( 1.0f - GameConstants::Sound::MinAdjustment );
      }

      // Compute Distance to eyePoint to adjust the volume based on that distance.
      XMVECTOR cameraToPosition = XMLoadFloat3( &eyePoint ) - VectorPosition( );
      float distToPositionSquared = XMVectorGetX( XMVector3LengthSq( cameraToPosition ) );

      float volume = min( distToPositionSquared, 1.0f );
      // Scale
      // Sound is proportional to how hard the ball is hitting.
      volume = adjustment * volume;

      m_hitSound->PlaySound( volume );
   }
}

//----------------------------------------------------------------------

/**
 * @brief
 *
 *
 * @details
 * @par
 * Derived classes must implement this method
 */
void TcObject::MUpdatePosition( void )
{
   // Nothing to update.
}
