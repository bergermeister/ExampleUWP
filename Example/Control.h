#pragma once

namespace Example
{
   private enum class TeStateUpdateEngine
   {
      XeWaitingForResources,
      XeResourcesLoaded,
      XeWaitingForPress,
      XeDynamics,
      XeTooSmall,
      XeSuspended,
      Deactivated
   };

   private enum class TeStatePressResult
   {
      XeLoadGame,
      XePlayLevel,
      XeContinueLevel
   };

   private enum class XeStateGameInfoOverlay
   {
      XeLoading,
      XeGameStats,
      XeGameOverExpired,
      XeGameOverCompleted,
      XeLevelStart,
      XePause
   };
};

