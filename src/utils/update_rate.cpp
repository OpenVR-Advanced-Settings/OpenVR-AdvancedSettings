#include "update_rate.h"

UpdateRate updateRate{};

/**
   @brief getSubjectRate returns the amount of frames before a subject is run
   again.
   @param Subject
   @return Amount of frames before subject is run again.

    So a return value of 19 would mean that every 19 frames, the subject is run.
    Divide the expected framerate with the return value to get the amount of
   times per second the subject is run.
 */
constexpr unsigned int getSubjectRate( const UpdateSubject subject )
{
    /*
       Return values are primes in order to reduce the amount of times where
       multiple subjects are run on the same frame.
    */
    switch ( subject )
    {
    case UpdateSubject::UtilitiesTabController:
        return 19;
    case UpdateSubject::VideoDashboard:
        return 47;
    case UpdateSubject::AudioTabController:
        return 89;
    case UpdateSubject::SteamVrTabController:
        return 97;
    case UpdateSubject::ChaperoneTabController:
        return 101;
    case UpdateSubject::SettingsTabController:
        return 157;
    }

    return 0;
}

bool UpdateRate::shouldSubjectRun( const UpdateSubject subject ) noexcept
{
    const auto subjectTarget = getSubjectRate( subject );

    const auto shouldRun = ( m_counter % subjectTarget ) == 0;

    return shouldRun;
}

bool UpdateRate::shouldSubjectNotRun( const UpdateSubject subject ) noexcept
{
    return !shouldSubjectRun( subject );
}

void UpdateRate::incrementCounter() noexcept
{
    m_counter++;
}
