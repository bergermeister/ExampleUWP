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

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Windows::Graphics::Display;
using namespace Example::NResources::NAudio;

TcAudio::TcAudio( void ) : vbAudioAvailable( false )
{

}

void TcAudio::MInitialize( void )
{

}

void TcAudio::MCreateDeviceIndependentResources( void )
{
   UINT32  kuiFlags = 0;
   HRESULT kiHR;

   DX::ThrowIfFailed( XAudio2Create( &this->voMusicEngine, kuiFlags ) );

   #if defined( _DEBUG )
   XAUDIO2_DEBUG_CONFIGURATION koDebugConfiguration = { 0 };
   koDebugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS;
   koDebugConfiguration.TraceMask = XAUDIO2_LOG_ERRORS;
   this->voMusicEngine->SetDebugConfiguration( &koDebugConfiguration );
   #endif

   kiHR = this->voMusicEngine->CreateMasteringVoice( &this->vopMusicMasteringVoice );
   if( FAILED( kiHR ) )
   {
      this->vbAudioAvailable = false;
   }
   else
   {
      DX::ThrowIfFailed( XAudio2Create( &this->voSoundEffectEngine, kuiFlags ) );

      #if defined( _DEBUG )
      this->voSoundEffectEngine->SetDebugConfiguration( &koDebugConfiguration );
      #endif

      DX::ThrowIfFailed( this->voSoundEffectEngine->CreateMasteringVoice( &this->vopSoundEffectMasteringVoice ) );
      
      this->vbAudioAvailable = true;
   }
}

IXAudio2* TcAudio::MMusicEngine( void )
{
   return( this->voMusicEngine.Get( ) );
}

IXAudio2* TcAudio::MSoundEffectEngine( void )
{
   return( this->voSoundEffectEngine.Get( ) );
}

void TcAudio::MSuspendAudio( void )
{
   if( this->vbAudioAvailable )
   {
      this->voMusicEngine->StopEngine( );
      this->voSoundEffectEngine->StopEngine( );
   }
}

void TcAudio::MResumeAudio( void )
{
   if( this->vbAudioAvailable )
   {
      DX::ThrowIfFailed( this->voMusicEngine->StartEngine( ) );
      DX::ThrowIfFailed( this->voSoundEffectEngine->StartEngine( ) );
   }
}
