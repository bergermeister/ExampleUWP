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

#include <pch.h>

using namespace Microsoft::WRL;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation;
using namespace Windows::ApplicationModel;
using namespace Example::NResources::NAudio;

TcMediaReader::TcMediaReader( void )
{
   ZeroMemory( &this->voWaveFormat, sizeof( WAVEFORMATEX ) );
   this->vopInstalledLocation = Package::Current->InstalledLocation;
   this->vopInstalledLocationPath = Platform::String::Concat( this->vopInstalledLocation->Path, "\\" );
}

WAVEFORMATEX* TcMediaReader::MGetOutputWaveFormatEx( void )
{
   return( &this->voWaveFormat );
}

Platform::Array< byte >^ TcMediaReader::MLoadMedia( _In_ Platform::String^ kopFilename )
{
   ComPtr< IMFSourceReader > koReader;
   ComPtr< IMFMediaType >    koMediaType;
   ComPtr< IMFMediaType >    koOutputMediaType;
   ComPtr<IMFSample>         koSample;
   ComPtr<IMFMediaBuffer>    koMediaBuffer;
   DWORD                     kuiFlags = 0;
   UINT32                    kuiSize = 0;
   UINT32                    kuiMaxLength;
   LONGLONG                  kulDuration;
   WAVEFORMATEX*             kopWaveFormat;
   PROPVARIANT               koPropVariant;
   Platform::Array<byte>^    kopFileData;
   BYTE*                     kucpAudioData = nullptr;
   DWORD                     kuiSampleBufferLength = 0;
   DWORD                     kuiIndex;
   int                       kiPositionInData = 0;
   bool                      kbDone = false;

   DX::ThrowIfFailed( MFStartup( MF_VERSION ) );   
   DX::ThrowIfFailed( MFCreateSourceReaderFromURL( Platform::String::Concat( this->vopInstalledLocationPath,
                                                                             kopFilename )->Data( ),
                                                                             nullptr,
                                                                             &koReader ) );
   DX::ThrowIfFailed( MFCreateMediaType( &koMediaType ) );
   DX::ThrowIfFailed( koMediaType->SetGUID( MF_MT_MAJOR_TYPE, MFMediaType_Audio ) );
   DX::ThrowIfFailed( koMediaType->SetGUID( MF_MT_SUBTYPE, MFAudioFormat_PCM ) );
   DX::ThrowIfFailed( koReader->SetCurrentMediaType( static_cast< uint32 >( MF_SOURCE_READER_FIRST_AUDIO_STREAM ),
                                                     0, koMediaType.Get( ) ) );
   DX::ThrowIfFailed( MFCreateWaveFormatExFromMFMediaType( koOutputMediaType.Get( ), &kopWaveFormat, &kuiSize ) );
   CopyMemory( &this->voWaveFormat, kopWaveFormat, sizeof( WAVEFORMATEX ) );
   CoTaskMemFree( kopWaveFormat );
   DX::ThrowIfFailed( koReader->GetPresentationAttribute( static_cast< uint32 >( MF_SOURCE_READER_MEDIASOURCE ),
                                                          MF_PD_DURATION,
                                                          &koPropVariant ) );

   // 'duration' is in 100ns units; convert to seconds, and round up to the nearest whole byte.
   kulDuration = koPropVariant.uhVal.QuadPart;
   kuiMaxLength = static_cast< UINT32 >( ( ( kulDuration * static_cast< ULONGLONG >( this->voWaveFormat.nAvgBytesPerSec ) ) +
                                             10000000 ) / 10000000 );

   kopFileData = ref new Platform::Array<byte>( kuiMaxLength );
  
   while( !kbDone )
   {
      DX::ThrowIfFailed( koReader->ReadSample( static_cast< uint32 >( MF_SOURCE_READER_FIRST_AUDIO_STREAM ), 0, nullptr, 
                                               &kuiFlags, nullptr, &koSample ) );

      if( koSample != nullptr )
      {
         DX::ThrowIfFailed( koSample->ConvertToContiguousBuffer( &koMediaBuffer ) );

         kucpAudioData = nullptr;
         kuiSampleBufferLength = 0;
         DX::ThrowIfFailed( koMediaBuffer->Lock( &kucpAudioData, nullptr, &kuiSampleBufferLength ) );

         for( kuiIndex = 0; kuiIndex < kuiSampleBufferLength; kuiIndex++ )
         {
            kopFileData[ kiPositionInData++ ] = kucpAudioData[ kuiIndex ];
         }
      }
      if( kuiFlags & MF_SOURCE_READERF_ENDOFSTREAM )
      {
         kbDone = true;
      }
   }

   return( kopFileData );
}
