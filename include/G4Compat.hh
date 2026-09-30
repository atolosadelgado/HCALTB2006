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


// ============================================================================
// Find a logical volume by name.
//
// This function is intended to be called AFTER the GDML geometry has been
// constructed/imported, when all G4LogicalVolumes are already in the
// G4LogicalVolumeStore.
//
// The GDML may contain names such as:
//
//     ECAL0x7f4588612700
//
// but the GDML parser removes the memory-address suffix when creating the
// G4LogicalVolume, so the corresponding G4LogicalVolume name is simply:
//
//     ECAL
//
// Therefore only the clean Geant4 name is searched here.
//
// If several logical volumes have the same name, all matches are reported.
// The first one is returned.
// ============================================================================

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ProductionCuts.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"

#include <vector>

inline G4LogicalVolume*
G4CompatFindLogicalVolume(const G4String& volumeName)
{
    auto* store = G4LogicalVolumeStore::GetInstance();

    if (!store)
    {
        G4cerr
            << "WARNING: G4LogicalVolumeStore is not available while "
               "looking for logical volume '"
            << volumeName << "'."
            << G4endl;

        MY_G4_FATAL("G4CompatFindLogicalVolume", "InvalidLogicalVolumeStore", "Logical Volume Store not found: " + volumeName);
    }

    G4LogicalVolume* result = nullptr;
    G4int nMatches = 0;

    for (auto* logicalVolume : *store)
    {
        if (!logicalVolume)
            continue;

        if (logicalVolume->GetName() == volumeName)
        {
            ++nMatches;

            if (!result)
            {
                result = logicalVolume;
            }
        }
    }

    if (nMatches == 0)
    {
        G4cerr
            << "WARNING: logical volume '"
            << volumeName
            << "' was not found."
            << G4endl;

        MY_G4_FATAL("G4CompatFindLogicalVolume", "InvalidLogicalVolume", "Logical volume not found: " + volumeName);
    }

    if (nMatches > 1)
    {
        G4cerr
            << "WARNING: found "
            << nMatches
            << " logical volumes named '"
            << volumeName
            << "'. Using the first one."
            << G4endl;
        MY_G4_WARNING("G4CompatFindLogicalVolume", "InvalidLogicalVolume", "Duplicated Logical volume: " + volumeName);
    }

    return result;
}


// ============================================================================
// Create a G4Region with identical production cuts for gamma, e-, e+ and
// protons.
//
// This corresponds to an old GDML block such as:
//
//   <auxiliary auxtype="Region" ...>
//     <auxiliary auxtype="volume" .../>
//     <auxiliary auxtype="gamcut" .../>
//     <auxiliary auxtype="ecut" .../>
//     <auxiliary auxtype="poscut" .../>
//     <auxiliary auxtype="pcut" .../>
//   </auxiliary>
//
// The region is attached to the specified logical volume as a root volume.
// ============================================================================

inline G4Region*
G4CompatCreateRegion(
    const G4String& regionName,
    const G4String& volumeName,
    G4double cut)
{
    // Avoid creating the same region twice.
    auto* regionStore = G4RegionStore::GetInstance();

    if (regionStore)
    {
        for (auto* region : *regionStore)
        {
            if (region && region->GetName() == regionName)
            {
                G4cerr
                    << "WARNING: region '"
                    << regionName
                    << "' already exists. "
                    << "Skipping creation."
                    << G4endl;

                MY_G4_WARNING("G4CompatCreateRegion", "InvalidRegion", "Duplicated region: " + regionName);
            }
        }
    }

    // Find the logical volume after the GDML geometry has been constructed.
    auto* logicalVolume =
        G4CompatFindLogicalVolume(volumeName);

    if (!logicalVolume)
    {
        G4cerr
            << "WARNING: cannot create region '"
            << regionName
            << "' because logical volume '"
            << volumeName
            << "' was not found."
            << G4endl;


        MY_G4_FATAL("G4CompatCreateRegion", "InvalidLogicalVolume", "Logical volume does not exist: " + volumeName);
    }

    // Create region.
    auto* region = new G4Region(regionName);

    // Create production cuts.
    auto* cuts = new G4ProductionCuts();

    cuts->SetProductionCut(cut, "gamma");
    cuts->SetProductionCut(cut, "e-");
    cuts->SetProductionCut(cut, "e+");
    cuts->SetProductionCut(cut, "proton");

    region->SetProductionCuts(cuts);

    // Attach the logical volume to the region.
    region->AddRootLogicalVolume(logicalVolume);

    return region;
}


// ============================================================================
// HCALTB2006 compatibility.
//
// Recreates the regions that are stored in the old GDML <userinfo> block:
//
//   OTBHCal  -> 10 mm
//   ECAL     ->  1 mm
//   TBHCal   ->  1 mm
//   HCal     ->  1 mm
//
// This function must be called AFTER the GDML geometry has been imported.
// ============================================================================

inline void G4CompatCreateHCALTB2006Regions()
{
    // Commented to avoid error triggered by kernel
    // it seems the region is only associated to Air material, not important
    // G4CompatCreateRegion(
    //     "DefaultRegionForTheWorld",
    //     "OTBHCal",
    //     10.0 * mm);

    G4CompatCreateRegion(
        "EcalRegion",
        "ECAL",
        1.0 * mm);

    G4CompatCreateRegion(
        "CaloRegion",
        "TBHCal",
        1.0 * mm);

    G4CompatCreateRegion(
        "HcalRegion",
        "HCal",
        1.0 * mm);
}

#endif
