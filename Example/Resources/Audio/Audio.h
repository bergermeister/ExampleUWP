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
         ref class TcAudio
         {
         protected private:
            bool                               vbAudioAvailable;
            Microsoft::WRL::ComPtr< IXAudio2 > voMusicEngine;
            Microsoft::WRL::ComPtr< IXAudio2 > voSoundEffectEngine;
            IXAudio2MasteringVoice* vopMusicMasteringVoice;
            IXAudio2MasteringVoice* vopSoundEffectMasteringVoice;

         internal:
            TcAudio( void );

            void MInitialize( void );
            void MCreateDeviceIndependentResources( void );
            IXAudio2* MMusicEngine( void );
            IXAudio2* MSoundEffectEngine( void );
            void MSuspendAudio( void );
            void MResumeAudio( void );
         };
      }
   }
}
