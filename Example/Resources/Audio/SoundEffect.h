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
      namespace NAudio
      {
         ref class TcSoundEffect
         {
         private:       // Private Attributes
            bool                     vbAudioAvailable;   ///< Audio Available Indication
            IXAudio2SourceVoice*     vopSourceVoice;     ///< Audio Source Pointer
            Platform::Array< byte >^ vopSoundData;       ///< Audio Data

         internal:      // Public Methods Available To Assembly
            TcSoundEffect( void );

            void MInitialize( _In_ IXAudio2*                aopMasteringEngine,
                              _In_ WAVEFORMATEX&            aorSourceFormat,
                              _In_ Platform::Array< byte >^ aopSoundData );
            void MPlay( _In_ float afVolume );
         };
      }
   }
}

