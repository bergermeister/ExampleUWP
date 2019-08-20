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
      namespace NObjects
      {
         /**
          * @brief
          *
          *
          * @details
          * @par
          * This class is the generic (abstract) representation of a D3D11 Indexed triangle list. Each of the derived classes
          * is the constructor for the specific geometry primitive. This abstract class does not place any requirements on the
          * format of the geometry directly.
          *
          * @par
          * The primary method of this class is MRender. The default implementation just sets the IndexBuffer, VertexBuffer,
          * and topology to a TriangleList and makes a DrawIndexed call on the context. It assumes all other state information
          * as already been set on the context.
          */
         ref class TcMesh abstract
         {
         private:       // Private Attributes
            Microsoft::WRL::ComPtr< ID3D11Buffer > vopVertexBuffer;
            Microsoft::WRL::ComPtr< ID3D11Buffer > vopIndexBuffer;
            int                                    viVertexCount;
            int                                    viIndexCount;

         internal:      // Public Methods Accessible To Assembly
            TcMesh( void );

            virtual void MRender( _In_ ID3D11DeviceContext& aorContext );
         };
      }
   }
}

