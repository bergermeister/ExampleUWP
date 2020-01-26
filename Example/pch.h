#pragma once

#include <wrl.h>
#include <wrl/client.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h>

#include <xaudio2.h>
#include <xaudio2fx.h>

#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>

#include <stdio.h>
#include <vector>
#include <memory>

#include <ppltasks.h>
#include <agile.h>
#include <concrt.h>

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

// Common DirectX
#include <Common/DirectXHelper.h>

// Constants
#include <Constants.h>

// Rendering Resources
#include <Resources/Rendering/Material.h>
#include <Resources/Rendering/ConstantBuffers.h>
#include <Resources/Rendering/Material.h>

// Audio Resources
#include <Resources/Audio/Audio.h>
#include <Resources/Audio/SoundEffect.h>
#include <Resources/Audio/MediaReader.h>

// Object Resources
#include <Resources/Objects/Mesh.h>
#include <Resources/Objects/Object.h>

// Animation Resources
#include <Resources/Animation/Animation.h>
#include <Resources/Animation/CirclePosition.h>
#include <Resources/Animation/LineListPosition.h>
#include <Resources/Animation/LinePosition.h>
#include <Resources/Animation/LineSegment.h>



