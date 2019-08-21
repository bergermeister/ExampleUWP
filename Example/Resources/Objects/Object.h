/**
 * @file
 * @brief
 *
 *
 * @details
 * @par
 */
#pragma once

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

            TcMaterial^ m_normalMaterial;
            TcMaterial^ m_hitMaterial;

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
