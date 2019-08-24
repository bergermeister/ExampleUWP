/**
 * @file
 * @brief
 *
 *
 * @details
 * @par
 */
#pragma once

#include <Resources/Objects/Mesh.h>
#include <Resources/Audio/SoundEffect.h>
#include <Resources/Animation/Animation.h>
#include <Resources/Rendering/Material.h>

namespace Example
{
   namespace NResources
   {
      namespace NObjects
      {
         ref class TcObject
         {
         protected private:       // Protected Attributes Accessible Within Assembly
            // Object Data
            bool                m_active;
            bool                m_target;
            int                 m_targetId;
            bool                m_hit;
            bool                m_ground;

            DirectX::XMFLOAT3   m_position;
            DirectX::XMFLOAT3   m_velocity;
            DirectX::XMFLOAT4X4 m_modelMatrix;

            NRendering::TcMaterial^ m_normalMaterial;
            NRendering::TcMaterial^ m_hitMaterial;

            DirectX::XMFLOAT3   m_defaultXAxis;
            DirectX::XMFLOAT3   m_defaultYAxis;
            DirectX::XMFLOAT3   m_defaultZAxis;

            float               m_hitTime;

            NAnimations::TcAnimation^ m_animatePosition;
            TcMesh^ m_mesh;

            NAudio::TcSoundEffect^ m_hitSound;

         internal:      // Public Methods Accessible Within Assembly
            TcObject( void );

            // Expect the IsTouching method to be overloaded by subclasses.
            virtual bool MIsTouching( DirectX::XMFLOAT3        aoPoint,
                                      float                    afRadius,
                                      _Out_ DirectX::XMFLOAT3& aorContact,
                                      _Out_ DirectX::XMFLOAT3& aorNormal );


            void MRender( _In_ ID3D11DeviceContext& aorContext,  _In_ ID3D11Buffer& aorPrimitiveConstantBuffer );

            void  MActive( bool abActive );
            bool  MActive( void );
            void  MTarget( bool abTarget );
            bool  MTarget( void );
            void  MHit( bool abHit );
            bool  MHit( void );
            void  MOnGround( bool aiGround );
            bool  MOnGround( void );
            void  MTargetId( int aiTargetId );
            int   MTargetId( void );
            void  MHitTime( float afT );
            float MHitTime( void );

            void MAnimate( _In_opt_ NAnimations::TcAnimation^ aopAnimation );
            NAnimations::TcAnimation^ MAnimate( void );

            void MHitSound( _In_ NAudio::TcSoundEffect^ aopHitSound );
            NAudio::TcSoundEffect^ MHitSound( void );

            void MPlaySound( float afImpactSpeed, DirectX::XMFLOAT3 aoEyePoint );

            void MMesh( _In_ TcMesh^ aopMesh );

            void MNormalMaterial( _In_ NRendering::TcMaterial^ aopMaterial );
            NRendering::TcMaterial^ MNormalMaterial( void );

            void MHitMaterial( _In_ NRendering::TcMaterial^ aopMaterial );
            NRendering::TcMaterial^ MHitMaterial( void );

            void MPosition( DirectX::XMFLOAT3 position );
            void MPosition( DirectX::XMVECTOR position );
            void MVelocity( DirectX::XMFLOAT3 velocity );
            void MVelocity( DirectX::XMVECTOR velocity );
            DirectX::XMMATRIX MModelMatrix( void );
            DirectX::XMFLOAT3 MPosition( void );
            DirectX::XMVECTOR MVectorPosition( void );
            DirectX::XMVECTOR MVectorVelocity( void );
            DirectX::XMFLOAT3 MVelocity( void );

         protected private:   // Protected Methods Accessible Within Assembly
            virtual void MUpdatePosition( void );
         };

         __forceinline void TcObject::MActive( bool abActive )
         {
            this->m_active = abActive;
         }

         __forceinline bool TcObject::MActive( void )
         {
            return( this->m_active );
         }

         __forceinline void TcObject::MTarget( bool abTarget )
         {
            this->m_target = abTarget;
         }

         __forceinline bool TcObject::MTarget( void )
         {
            return( this->m_target );
         }

         __forceinline void TcObject::MHit( bool abHit )
         {
            this->m_hit = abHit;
         }

         __forceinline bool TcObject::MHit( void )
         {
            return( this->m_hit );
         }

         __forceinline void TcObject::MOnGround( bool abGround )
         {
            this->m_ground = abGround;
         }

         __forceinline bool TcObject::MOnGround( void )
         {
            return( this->m_ground );
         }

         __forceinline void TcObject::MTargetId( int aiTargetId )
         {
            this->m_targetId = aiTargetId;
         }

         __forceinline int TcObject::MTargetId( void )
         {
            return( this->m_targetId );
         }

         __forceinline void TcObject::MHitTime( float afT )
         {
            this->m_hitTime = afT;
         }

         __forceinline float TcObject::MHitTime( void )
         {
            return( this->m_hitTime );
         }

         __forceinline void TcObject::MPosition( DirectX::XMFLOAT3 aoPosition )
         {
            this->m_position = aoPosition;
            // Update any internal states that are dependent on the position.
            // UpdatePosition is a virtual function that is specific to the derived class.
            this->MUpdatePosition( );
         }

         __forceinline void TcObject::MPosition( DirectX::XMVECTOR aoPosition )
         {
            XMStoreFloat3( &this->m_position, aoPosition );
            // Update any internal states that are dependent on the position.
            // UpdatePosition is a virtual function that is specific to the derived class.
            this->MUpdatePosition( );
         }

         __forceinline DirectX::XMFLOAT3 TcObject::MPosition( void )
         {
            return( this->m_position );
         }

         __forceinline DirectX::XMVECTOR TcObject::MVectorPosition( void )
         {
            return( DirectX::XMLoadFloat3( &this->m_position ) );
         }

         __forceinline void TcObject::MVelocity( DirectX::XMFLOAT3 aoVelocity )
         {
            this->m_velocity = aoVelocity;
         }

         __forceinline void TcObject::MVelocity( DirectX::XMVECTOR aoVelocity )
         {
            XMStoreFloat3( &this->m_velocity, aoVelocity );
         }

         __forceinline DirectX::XMFLOAT3 TcObject::MVelocity( void )
         {
            return( this->m_velocity );
         }

         __forceinline DirectX::XMVECTOR TcObject::MVectorVelocity( void )
         {
            return( DirectX::XMLoadFloat3( &this->m_velocity ) );
         }

         __forceinline void TcObject::MAnimate( _In_opt_ NAnimations::TcAnimation^ aopAnimation )
         {
            this->m_animatePosition = aopAnimation;
         }

         __forceinline NAnimations::TcAnimation^ TcObject::MAnimate( void )
         {
            return( this->m_animatePosition );
         }

         __forceinline void TcObject::MNormalMaterial( _In_ NRendering::TcMaterial^ aopMaterial )
         {
            this->m_normalMaterial = aopMaterial;
         }

         __forceinline NRendering::TcMaterial^ TcObject::MNormalMaterial( void )
         {
            return( this->m_normalMaterial );
         }

         __forceinline void TcObject::MHitMaterial( _In_ NRendering::TcMaterial^ aopMaterial )
         {
            this->m_hitMaterial = aopMaterial;
         }

         __forceinline NRendering::TcMaterial^ TcObject::MHitMaterial( void )
         {
            return( this->m_hitMaterial );
         }

         __forceinline void TcObject::MMesh( _In_ TcMesh^ aopMesh )
         {
            this->m_mesh = aopMesh;
         }

         __forceinline void TcObject::MHitSound( _In_ NAudio::TcSoundEffect^ aopHitSound )
         {
            this->m_hitSound = aopHitSound;
         }

         __forceinline NAudio::TcSoundEffect^ TcObject::MHitSound( void )
         {
            return( this->m_hitSound );
         }

         __forceinline DirectX::XMMATRIX TcObject::MModelMatrix( void )
         {
            return DirectX::XMLoadFloat4x4( &this->m_modelMatrix );
         }
      }
   }
}
