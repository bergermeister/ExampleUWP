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
         ref class TcMediaReader
         {
         protected private:
            Windows::Storage::StorageFolder^ vopInstalledLocation;
            Platform::String^                vopInstalledLocationPath;
            WAVEFORMATEX                     voWaveFormat;

         internal:
            TcMediaReader( void );

            Platform::Array< byte >^ MLoadMedia( _In_ Platform::String^ kopFilename );
            WAVEFORMATEX*            MGetOutputWaveFormatEx( void );
         };
      }
   }
}

