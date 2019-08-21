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
#include <Resources/Audio/SoundEffect.h>
#include <Common/DirectXHelper.h>

using namespace Example::NResources::NAudio;

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
TcSoundEffect::TcSoundEffect( void ) : vbAudioAvailable( false )
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
void TcSoundEffect::MInitialize( _In_ IXAudio2*                aopMasteringEngine,
                                 _In_ WAVEFORMATEX&            aorSourceFormat,
                                 _In_ Platform::Array< byte >^ aopSoundData )
{
   this->vopSoundData = aopSoundData;

   if( aopMasteringEngine == nullptr )
   {
      this->vbAudioAvailable = false;
   }
   else
   {
      DX::ThrowIfFailed( aopMasteringEngine->CreateSourceVoice( &this->vopSourceVoice, &aorSourceFormat ) );
      this->vbAudioAvailable = true;
   }
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
void TcSoundEffect::MPlay( _In_ float afVolume )
{
   XAUDIO2_BUFFER koBuffer = { 0 };

   if( this->vbAudioAvailable )
   {
      // Interrupt sound effect if it is currently playing
      DX::ThrowIfFailed( this->vopSourceVoice->Stop( ) );
      DX::ThrowIfFailed( this->vopSourceVoice->FlushSourceBuffers( ) );

      // Queue the memory buffer for playback and start the voice
      koBuffer.AudioBytes = this->vopSoundData->Length;
      koBuffer.pAudioData = this->vopSoundData->Data;
      koBuffer.Flags      = XAUDIO2_END_OF_STREAM;

      DX::ThrowIfFailed( this->vopSourceVoice->SetVolume( afVolume ) );
      DX::ThrowIfFailed( this->vopSourceVoice->SubmitSourceBuffer( &koBuffer ) );
      DX::ThrowIfFailed( this->vopSourceVoice->Start( ) );
   }
}

