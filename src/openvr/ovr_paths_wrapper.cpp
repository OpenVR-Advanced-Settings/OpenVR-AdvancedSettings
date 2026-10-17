#include "ovr_paths_wrapper.h"
#include <QtLogging>
#include <QtDebug>
#include <cstdint>

//Reverse Engineered interface for IVRpaths from openvr specifically 002
namespace ovr_paths_wrapper
{
namespace
{
    using PathHandle_t = uint64_t;

    enum EPropertyWriteType
    {
        PropertyWrite_Set = 0,
        PropertyWrite_Erase = 1,
        PropertyWrite_SetError = 2,
    };

    struct PathRead_t
    {
        PathHandle_t ulPath;
        void* pvBuffer;
        uint32_t unBufferSize;
        vr::PropertyTypeTag_t unTag;
        uint32_t unRequiredBufferSize;
        vr::ETrackedPropertyError eError;
        const char* pszPath;
    };

    struct PathWrite_t
    {
        PathHandle_t ulPath;
        EPropertyWriteType writeType;
        vr::ETrackedPropertyError eSetError;
        void* pvBuffer;
        uint32_t unBufferSize;
        vr::PropertyTypeTag_t unTag;
        vr::ETrackedPropertyError eError;
        const char* pszPath;
        bool bPostEvents;
        bool bValueChanged;
    };

    class IVRPaths
    {
    public:
        virtual vr::ETrackedPropertyError
            ReadPathBatch( vr::PropertyContainerHandle_t ulRootHandle,
                           PathRead_t* pBatch,
                           uint32_t unBatchEntryCount )
            = 0;
        virtual vr::ETrackedPropertyError
            WritePathBatch( vr::PropertyContainerHandle_t ulRootHandle,
                            PathWrite_t* pBatch,
                            uint32_t unBatchEntryCount )
            = 0;
        virtual vr::ETrackedPropertyError
            StringToHandle( PathHandle_t* pHandle, const char* pchPath )
            = 0;
        virtual vr::ETrackedPropertyError
            HandleToString( PathHandle_t pHandle,
                            char* pchBuffer,
                            uint32_t unBufferSize,
                            uint32_t* punBufferSizeUsed )
            = 0;
    };

    constexpr auto k_pathsInterfaceVersion = "IVRPaths_002";
    // Root container vrclient passes to ReadPathBatch for the chaperone paths
    constexpr vr::PropertyContainerHandle_t k_chaperoneRoot
        = 0x0000000600000003ULL;
    constexpr auto k_standingTransformPath = "/chaperone/standing/transform";
    constexpr auto k_seatedTransformPath = "/chaperone/seated/transform";

    IVRPaths* g_paths = nullptr;
    bool g_initFailed = false;
    PathHandle_t g_standingHandle = 0;
    PathHandle_t g_seatedHandle = 0;

    const char* errorName( vr::ETrackedPropertyError error )
    {
        return vr::VRSystem() ? vr::VRSystem()->GetPropErrorNameFromEnum( error )
                              : "unknown";
    }

    // Loads the interface and resolves the path handles once
    bool init()
    {
        if ( g_paths )
        {
            return true;
        }
        if ( g_initFailed || !vr::VRSystem() )
        {
            return false;
        }

        vr::EVRInitError initError = vr::VRInitError_None;
        auto paths = static_cast<IVRPaths*>(
            vr::VR_GetGenericInterface( k_pathsInterfaceVersion, &initError ) );
        if ( !paths || initError != vr::VRInitError_None )
        {
            g_initFailed = true;
            qWarning() << k_pathsInterfaceVersion
                       << "not provided by the runtime";
            return false;
        }
        if ( paths->StringToHandle( &g_standingHandle, k_standingTransformPath )
                 != vr::TrackedProp_Success
             || paths->StringToHandle( &g_seatedHandle, k_seatedTransformPath )
                    != vr::TrackedProp_Success )
        {
            g_initFailed = true;
            qWarning() << "Could not resolve the chaperone transform paths";
            return false;
        }
        g_paths = paths;
        return true;
    }

    PathsError readTransform( PathHandle_t handle,
                              const char* path,
                              vr::HmdMatrix34_t& transform )
    {
        PathRead_t read = {};
        read.ulPath = handle;
        read.pvBuffer = &transform;
        read.unBufferSize = sizeof( transform );
        read.pszPath = path;
        vr::ETrackedPropertyError batchError
            = g_paths->ReadPathBatch( k_chaperoneRoot, &read, 1 );
        if ( batchError != vr::TrackedProp_Success
             || read.eError != vr::TrackedProp_Success
             || read.unTag != vr::k_unHmdMatrix34PropertyTag
             || read.unRequiredBufferSize != sizeof( transform ) )
        {
            qInfo() << "Reading" << path << "failed: batch"
                    << errorName( batchError ) << "entry"
                    << errorName( read.eError ) << "tag" << read.unTag
                    << "size" << read.unRequiredBufferSize;
            return PathsError::ReadFailed;
        }
        return PathsError::NoError;
    }

    PathsError writeTransform( PathHandle_t handle,
                               const char* path,
                               const vr::HmdMatrix34_t& transform )
    {
        vr::HmdMatrix34_t buffer = transform;
        PathWrite_t write = {};
        write.ulPath = handle;
        write.writeType = PropertyWrite_Set;
        write.pvBuffer = &buffer;
        write.unBufferSize = sizeof( buffer );
        write.unTag = vr::k_unHmdMatrix34PropertyTag;
        write.pszPath = path;
        write.bPostEvents = false;
        vr::ETrackedPropertyError batchError
            = g_paths->WritePathBatch( k_chaperoneRoot, &write, 1 );
        if ( batchError != vr::TrackedProp_Success
             || write.eError != vr::TrackedProp_Success )
        {
            qWarning() << "Writing" << path << "failed: batch"
                       << errorName( batchError ) << "entry"
                       << errorName( write.eError );
            return PathsError::WriteFailed;
        }
        return PathsError::NoError;
    }
} // namespace

PathsError readStandingTransform( vr::HmdMatrix34_t& transform )
{
    if ( !init() )
    {
        return PathsError::Unavailable;
    }
    return readTransform( g_standingHandle, k_standingTransformPath, transform );
}

PathsError readSeatedTransform( vr::HmdMatrix34_t& transform )
{
    if ( !init() )
    {
        return PathsError::Unavailable;
    }
    return readTransform( g_seatedHandle, k_seatedTransformPath, transform );
}

PathsError writeStandingTransform( const vr::HmdMatrix34_t& transform )
{
    if ( !init() )
    {
        return PathsError::Unavailable;
    }
    return writeTransform(
        g_standingHandle, k_standingTransformPath, transform );
}

PathsError writeSeatedTransform( const vr::HmdMatrix34_t& transform )
{
    if ( !init() )
    {
        return PathsError::Unavailable;
    }
    return writeTransform( g_seatedHandle, k_seatedTransformPath, transform );
}
} // namespace ovr_paths_wrapper
