// ----------------------------------------------------------------------------
// PolicyConfig.h
// Undocumented COM-interface IPolicyConfig.
// Use for set default audio render endpoint
// @author EreTIk
// ----------------------------------------------------------------------------

#pragma once

// ca286fc3-91fd-42c3-8e9b-caafa66242e3
static const GUID IID_IPolicyConfig2
    = { 0xCA286FC3,
        0x91FD,
        0x42C3,
        { 0x8E, 0x9B, 0xCA, 0xAF, 0xA6, 0x62, 0x42, 0xE3 } };

// 6be54be8-a068-4875-a49d-0c2966473b11
static const GUID IID_IPolicyConfig1
    = { 0x6BE54BE8,
        0xA068,
        0x4875,
        { 0xA4, 0x9D, 0x0C, 0x29, 0x66, 0x47, 0x3B, 0x11 } };

// f8679f50-850a-41cf-9c72-430f290290c8
static const GUID IID_IPolicyConfig0
    = { 0xF8679F50,
        0x850A,
        0x41CF,
        { 0x9C, 0x72, 0x43, 0x0F, 0x29, 0x02, 0x90, 0xC8 } };

class DECLSPEC_UUID( "870af99c-171d-4f9e-af0d-e63df40c2bc9" )
    CPolicyConfigClient;

interface IPolicyConfig : public IUnknown
{
public:
    virtual HRESULT GetMixFormat( PCWSTR, WAVEFORMATEX** );

    virtual HRESULT STDMETHODCALLTYPE GetDeviceFormat(
        PCWSTR, INT, WAVEFORMATEX** );

    virtual HRESULT STDMETHODCALLTYPE ResetDeviceFormat( PCWSTR );

    virtual HRESULT STDMETHODCALLTYPE SetDeviceFormat(
        PCWSTR, WAVEFORMATEX*, WAVEFORMATEX* );

    virtual HRESULT STDMETHODCALLTYPE GetProcessingPeriod(
        PCWSTR, INT, PINT64, PINT64 );

    virtual HRESULT STDMETHODCALLTYPE SetProcessingPeriod( PCWSTR, PINT64 );

    virtual HRESULT STDMETHODCALLTYPE GetShareMode( PCWSTR,
                                                    struct DeviceShareMode* );

    virtual HRESULT STDMETHODCALLTYPE SetShareMode( PCWSTR,
                                                    struct DeviceShareMode* );

    virtual HRESULT STDMETHODCALLTYPE GetPropertyValue(
        PCWSTR, int, const PROPERTYKEY&, const PROPVARIANT* );

    virtual HRESULT STDMETHODCALLTYPE SetPropertyValue(
        PCWSTR, int, const PROPERTYKEY&, const PROPVARIANT* );

    virtual HRESULT STDMETHODCALLTYPE SetDefaultEndpoint(
        __in PCWSTR wszDeviceId, __in ERole eRole );

    virtual HRESULT STDMETHODCALLTYPE SetEndpointVisibility( PCWSTR, INT );
};

interface DECLSPEC_UUID( "568b9108-44bf-40b4-9006-86afe5b5a620" )
    IPolicyConfigVista;
class DECLSPEC_UUID( "294935CE-F637-4E7C-A41B-AB255460B862" )
    CPolicyConfigVistaClient;
// ----------------------------------------------------------------------------
// class CPolicyConfigVistaClient
// {294935CE-F637-4E7C-A41B-AB255460B862}
//
// interface IPolicyConfigVista
// {568b9108-44bf-40b4-9006-86afe5b5a620}
//
// Query interface:
// CComPtr<IPolicyConfigVista> PolicyConfig;
// PolicyConfig.CoCreateInstance(__uuidof(CPolicyConfigVistaClient));
//
// @compatible: Windows Vista and Later
// ----------------------------------------------------------------------------
interface IPolicyConfigVista : public IUnknown
{
public:
    virtual HRESULT GetMixFormat(
        PCWSTR,
        WAVEFORMATEX** ); // not available on Windows 7, use method from
                          // IPolicyConfig

    virtual HRESULT STDMETHODCALLTYPE GetDeviceFormat(
        PCWSTR, INT, WAVEFORMATEX** );

    virtual HRESULT STDMETHODCALLTYPE SetDeviceFormat(
        PCWSTR, WAVEFORMATEX*, WAVEFORMATEX* );

    virtual HRESULT STDMETHODCALLTYPE GetProcessingPeriod(
        PCWSTR,
        INT,
        PINT64,
        PINT64 ); // not available on Windows 7, use method from IPolicyConfig

    virtual HRESULT STDMETHODCALLTYPE SetProcessingPeriod(
        PCWSTR,
        PINT64 ); // not available on Windows 7, use method from IPolicyConfig

    virtual HRESULT STDMETHODCALLTYPE GetShareMode(
        PCWSTR,
        struct DeviceShareMode* ); // not available on Windows 7, use method
                                   // from IPolicyConfig

    virtual HRESULT STDMETHODCALLTYPE SetShareMode(
        PCWSTR,
        struct DeviceShareMode* ); // not available on Windows 7, use method
                                   // from IPolicyConfig

    virtual HRESULT STDMETHODCALLTYPE GetPropertyValue(
        PCWSTR, int, const PROPERTYKEY&, const PROPVARIANT* );

    virtual HRESULT STDMETHODCALLTYPE SetPropertyValue(
        PCWSTR, int, const PROPERTYKEY&, const PROPVARIANT* );

    virtual HRESULT STDMETHODCALLTYPE SetDefaultEndpoint(
        __in PCWSTR wszDeviceId, __in ERole eRole );

    virtual HRESULT STDMETHODCALLTYPE SetEndpointVisibility(
        PCWSTR,
        INT ); // not available on Windows 7, use method from IPolicyConfig
};
