#pragma once

#include <memory>
#include <mutex>
#include <openvr.h>
#include <cmath>

namespace utils
{
class ChaperoneUtils
{
private:
    std::recursive_mutex _mutex;
    uint32_t _quadsCount = 0;
    std::unique_ptr<vr::HmdVector3_t> _corners;
    bool _chaperoneWellFormed = true;
    void _getDistancesToChaperone( const vr::HmdVector3_t& point,
		    		  float* distances,
		    		  vr::HmdVector3_t* projectedPoints);

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

    void getDistancesToChaperone( const vr::HmdVector3_t& point,
		    		  float* distances,
		    		  vr::HmdVector3_t* projectedPoints = nullptr,
				  bool doLock = false )
    {
        if ( doLock )
        {
            std::lock_guard<std::recursive_mutex> lock( _mutex );
            return _getDistancesToChaperone( point, distances, projectedPoints );
        }
        else
        {
            return _getDistancesToChaperone( point, distances, projectedPoints );
        }
    }

    float getDistanceToChaperone( const vr::HmdVector3_t& point,
                                  vr::HmdVector3_t* projectedPoint = nullptr,
                                  bool doLock = false )
    {
	 std::unique_ptr<vr::HmdVector3_t[]> points(new vr::HmdVector3_t[_quadsCount]);
	 std::unique_ptr<float[]> distances(new float[_quadsCount]);
	 getDistancesToChaperone( point, distances.get(), points.get(), doLock);
	 size_t idx = 0;
	 for(size_t i = 0; i < _quadsCount; i++)
	 {
	    if(std::isnan(distances[i]) || (distances[i] < distances[idx])) idx = i;
	 }
	 if(projectedPoint) *projectedPoint = points[idx];
	 
	 return distances[idx];
    }
};

} // end namespace utils
