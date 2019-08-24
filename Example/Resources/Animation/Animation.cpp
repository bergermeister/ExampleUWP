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
#include <Resources/Animation/Animation.h>

using namespace Example::NResources::NAnimation;

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
TcAnimation::TcAnimation( void ) : vbContinuous( false ),
                                   vfStartTime( 0.0f ),
                                   vfDuration( 10.0f )
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
bool TcAnimation::MIsActive( _In_ float afT ) const
{
   return( ( afT >= this->vfStartTime ) && ( this->vbContinuous || ( afT + this->vfDuration ) ) );
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
bool TcAnimation::MIsFinished( _In_ float afT ) const
{
   return( !this->vbContinuous && ( afT >= ( this->vfStartTime + this->vfDuration ) ) );
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
float TcAnimation::MStart( void ) const
{
   return( this->vfStartTime );
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
void TcAnimation::MStart( _In_ float afStart )
{
   this->vfStartTime = afStart;
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
float TcAnimation::MDuration( void ) const
{
   return( this->vfDuration );
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
void TcAnimation::MDuration( _In_ float afDuration )
{
   this->vfDuration = afDuration;
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
bool TcAnimation::MContinuous( void ) const
{
   return( this->vbContinuous );
}

/**
 * @brief
 *
 *
 * @details
 * @par
 *
 */
void TcAnimation::MContinuous( _In_ bool abContinuous )
{
   this->vbContinuous = abContinuous;
}
