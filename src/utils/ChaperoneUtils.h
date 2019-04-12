#pragma once

#include <memory>
#include <mutex>
#include <openvr.h>

namespace utils
{
class ChaperoneUtils
{
private:
    std::recursive_mutex _mutex;
    uint32_t _quadsCount = 0;
    std::unique_ptr<vr::HmdVector3_t> _corners;
    bool _chaperoneWellFormed = true;

    float _getDistanceToChaperone( const vr::HmdVector3_t& point,
                                   vr::HmdVector3_t* projectedPoint );

public:
    uint32_t quadsCount() const noexcept
    {
        return _quadsCount;
    }
    bool isChaperoneWellFormed() const noexcept
    {
        return _chaperoneWellFormed;
    }

    std::recursive_mutex& mutex() noexcept
    {
        return _mutex;
    }

    void loadChaperoneData( bool fromLiveBounds = true );

    float getDistanceToChaperone( const vr::HmdVector3_t& point,
                                  vr::HmdVector3_t* projectedPoint = nullptr,
                                  bool doLock = false )
    {
        if ( doLock )
        {
            std::lock_guard<std::recursive_mutex> lock( _mutex );
            return _getDistanceToChaperone( point, projectedPoint );
        }
        else
        {
            return _getDistanceToChaperone( point, projectedPoint );
        }
    }
};

} // end namespace utils
