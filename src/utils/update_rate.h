#pragma once

enum class UpdateSubject
{
    AudioTabController,
    ChaperoneTabController,
    SettingsTabController,
    SteamVrTabController,
    UtilitiesTabController,
    VideoDashboard,
};

class UpdateRate
{
public:
    [[nodiscard]] bool shouldSubjectRun( const UpdateSubject subject ) noexcept;
    [[nodiscard]] bool
        shouldSubjectNotRun( const UpdateSubject subject ) noexcept;
    void incrementCounter() noexcept;

private:
    // counter is deliberately set as unsigned so that the program can continue
    // functioning if the counter should go above INT_MAX (or UINT_MAX in this
    // case)
    unsigned int m_counter = 0;
};

extern UpdateRate updateRate;
