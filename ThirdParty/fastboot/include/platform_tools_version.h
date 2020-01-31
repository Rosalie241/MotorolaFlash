#ifndef PLATFORM_TOOLS_VERSION_H
#define PLATFORM_TOOLS_VERSION_H

#define PLATFORM_TOOLS_VERSION "CUSTOM/MOTOROLAFLASH"

namespace android
{
class build
{
  public:
    static std::string GetBuildNumber()
    {
        return PLATFORM_TOOLS_VERSION;
    }
};
} // namespace android

#endif // PLATFORM_TOOLS_VERSION_H