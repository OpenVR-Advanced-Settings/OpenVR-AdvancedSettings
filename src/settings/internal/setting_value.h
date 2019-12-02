#pragma once
#include "settings_internal.h"

namespace settings
{
class SettingValue
{
public:
    SettingValue( const SettingCategory category, const QtInfo qtInfo ) noexcept
        : m_category{ category }, m_qtInfo{ qtInfo }
    {
    }

    virtual ~SettingValue() {}

    [[nodiscard]] SettingCategory category() const noexcept
    {
        return m_category;
    }

    [[nodiscard]] QtInfo qtInfo() const noexcept
    {
        return m_qtInfo;
    }

    virtual void saveValue() = 0;

private:
    const SettingCategory m_category;
    const QtInfo m_qtInfo;
};

} // namespace settings
