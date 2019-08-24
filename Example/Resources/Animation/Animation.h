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
      namespace NAnimation
      {
         /**
          * @brief
          *
          *
          * @details
          * @par
          * This is an abstract class for animations. It defines a set of capabilities for animating XMFLOAT3 variables.
          * An animation has the following characteristics:
          * - Start - The time for the animation to start
          * - Duration - The length of time the animation is to run
          * - Continuous - Whether the animation loops after duration is reached or just stops
          * Tere are two query functions:
          * - IsActive - Determines if the animation is active at time t
          * - IsFinished - Determines if the animation is done at time t
          * It is expected that each derived class will provide an Evaluate method for the specific kind of animation.
          */
         ref class TcAnimation abstract
         {
         protected private:       // Private Attributes
            bool  vbContinuous;  ///< If true, cycle to beginning at end
            float vfStartTime;   ///< Time at which animation begins
            float vfDuration;    ///< 1 cycle through path

         internal:      // Public Methods Accessible to Assembly
            TcAnimation( void );

            virtual DirectX::XMFLOAT3 MEvaluate( _In_ float afT ) = 0;

            bool  MIsActive( _In_ float afT ) const;
            bool  MIsFinished( _In_ float afT ) const;
            float MStart( void ) const;
            void  MStart( _In_ float afStart );
            float MDuration( void ) const;
            void  MDuration( _In_ float afDuration );
            bool  MContinuous( void ) const;
            void  MContinuous( _In_ bool abContinuous );
         };
      }
   }
}