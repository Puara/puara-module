#pragma once

#if !__has_include(<puara-application-settings.hpp>)
#error The project must have a puara-application-config.hpp file that defines the puara_application_settings structure.
struct puara_application_settings
{ 
    
} settings;
#endif

#include <puara-application-settings.hpp>