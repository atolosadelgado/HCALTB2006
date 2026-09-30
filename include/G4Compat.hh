#ifndef G4COMPAT_HH
#define G4COMPAT_HH

#include "G4Version.hh"

#if G4VERSION_NUMBER < 1070

#include <iostream>
#include <cstdlib>

#define MY_G4_WARNING(origin, code, msg) \
    do { \
        std::cerr << "WARNING [" << origin << "] " \
                  << code << ": " << msg << std::endl; \
    } while (0)

#define MY_G4_FATAL(origin, code, msg) \
    do { \
        std::cerr << "FATAL [" << origin << "] " \
                  << code << ": " << msg << std::endl; \
        std::abort(); \
    } while (0)

#else

#include "G4Exception.hh"

#define MY_G4_WARNING(origin, code, msg) \
    do { \
        G4ExceptionDescription g4msg; \
        g4msg << msg; \
        G4Exception(origin, code, JustWarning, g4msg); \
    } while (0)

#define MY_G4_FATAL(origin, code, msg) \
    do { \
        G4ExceptionDescription g4msg; \
        g4msg << msg; \
        G4Exception(origin, code, FatalException, g4msg); \
    } while (0)

#endif

#include "G4Threading.hh"

inline G4bool IsMasterThreadCompat()
{
#if G4VERSION_NUMBER >= 1020
    return G4Threading::IsMasterThread();
#else
    return G4Threading::G4GetThreadId() == G4Threading::MASTER_ID;
#endif
}


#include "tools/histo/h1d"
inline void get_bin_content_compat(
    tools::histo::h1d* h,
    unsigned int i,
    unsigned int& entries,
    double& Sw,
    double& Sw2,
    double& Sxw,
    double& Sx2w)
{

#if G4VERSION_NUMBER >= 1020
    h->get_bin_content(i, entries, Sw, Sw2, Sxw, Sx2w);
#else
    const tools::histo::h1d::hd_t data = h->get_histo_data();

    entries = data.m_bin_entries[i];
    Sw      = data.m_bin_Sw[i];
    Sw2     = data.m_bin_Sw2[i];
    Sxw     = data.m_bin_Sxw[i][0];
    Sx2w    = data.m_bin_Sx2w[i][0];
#endif
}

#endif
