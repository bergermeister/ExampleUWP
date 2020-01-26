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

//using namespace Microsoft::WRL;
//using namespace DirectX;
using namespace Example::NResources;

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
NObjects::TcMesh::TcMesh( void ) : viVertexCount( 0 ), viIndexCount( 0 )
{

}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
void NObjects::TcMesh::MRender( _In_ ID3D11DeviceContext& aorContext )
{
   uint32 kuiStride = sizeof( NRendering::TsPNTVertex );
   uint32 kuiOffset = 0;

   aorContext.IASetVertexBuffers( 0, 1, this->vopVertexBuffer.GetAddressOf( ), &kuiStride, &kuiOffset );
   aorContext.IASetIndexBuffer( this->vopIndexBuffer.Get( ), DXGI_FORMAT_R16_UINT, 0 );
   aorContext.IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
   aorContext.DrawIndexed( this->viIndexCount, 0, 0 );
}
