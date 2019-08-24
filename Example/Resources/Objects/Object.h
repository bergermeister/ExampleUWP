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
         private:       // Private Attributes
            virtual void UpdatePosition( ) {};
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

            Animate^ m_animatePosition;
            MeshObject^ m_mesh;

            SoundEffect^ m_hitSound;

         internal:      // Public Methods Accessible to Assembly
            TcObject( void );
         };
      }
   }
}
