import ROOT

ROOT.gROOT.SetBatch(True)

import math


# -------------------------------------------------------------------------
# Particle masses
# -------------------------------------------------------------------------

pname_pmassGeV = {
    "pi-": 0.1396,
    "pi+": 0.1396,
    "proton": 0.9383,
    "anti_proton": 0.9383,
    "kaon+": 0.4937,
    "kaon-": 0.4937,
    "e-": 0.000511,
}


# -------------------------------------------------------------------------
# Calibration and smearing constants
# -------------------------------------------------------------------------

MeV_to_GeV = 1.e-3

# Passed as arguments when launching the script
# Default values as used by CMSSW 15.0.10
ECAL_calibrationFactor = 1.01
HCAL_calibrationFactor = 106.5
ECAL_smearing = 0.362  # GeV
HCAL_smearing = 0.64   # GeV


# -------------------------------------------------------------------------
# Functions
# -------------------------------------------------------------------------

def ComputeMomentum(ekin_GeV, pname):

    mass_GeV = pname_pmassGeV[pname]

    return math.sqrt(
        ekin_GeV * (ekin_GeV + 2.0 * mass_GeV)
    )


def processECAL(e_ecal):

    return (
        MeV_to_GeV
        * ECAL_calibrationFactor
        * e_ecal
        + ROOT.gRandom.Gaus(0.0, ECAL_smearing)
    )


def processHCAL(e_hcal):

    return (
        MeV_to_GeV
        * HCAL_calibrationFactor
        * e_hcal
        + ROOT.gRandom.Gaus(0.0, HCAL_smearing)
    )


def fitAroundMaximum(h, nBinsAround):

    if h is None or h.GetEntries() == 0:
        print("Cannot fit histogram: empty or null histogram")
        return False, 0.0, 0.0

    # Find bin with maximum counts
    maxBin = h.GetMaximumBin()

    # Define range for fitting
    binLow = max(1, maxBin - nBinsAround)
    binHigh = min(h.GetNbinsX(), maxBin + nBinsAround)

    # Convert bin range to x coordinates
    xLow = h.GetBinLowEdge(binLow)
    xHigh = h.GetBinLowEdge(binHigh + 1)

    # Define fit function
    f = ROOT.TF1("f", "gaus", xLow, xHigh)

    # Initialize parameters close to expected result
    f.SetParameters(
        h.GetMaximum(),
        h.GetBinCenter(maxBin),
        h.GetRMS()
    )

    # Fit
    # Q = quiet
    # R = use specified range
    # + = keep function attached to histogram
    fitStatus = h.Fit(f, "QR+")

    if int(fitStatus) != 0:
        print(
            f"Gaussian fit failed with status {int(fitStatus)}"
        )
        return False, 0.0, 0.0

    mean = f.GetParameter(1)
    sigma = f.GetParameter(2)
    f.Draw("same")

    return True, mean, sigma


# -------------------------------------------------------------------------
# Main analysis function
# -------------------------------------------------------------------------

def compute_response(
    pname="pi-",
    penergy_GeV=49.861,
    xmin=20.0,
    xmax=80.0,
    nevents=20000,
    airECAL=0
):

    # Use a fixed seed for reproducibility
    ROOT.gRandom.SetSeed(12345)

    standalone_filename = (
        f"HCALTB2006_{pname}_{penergy_GeV:.6f}_"
        f"{nevents}evt_CMS_airECAL{airECAL}.root"
    )

    file = ROOT.TFile.Open(standalone_filename)

    if file is None or file.IsZombie():
        print(
            f"File <{standalone_filename}> "
            "could not be opened"
        )
        return

    tree = file.Get("tree")

    if tree is None:
        print(
            f"Could not find TTree <tree> "
            f"in file <{standalone_filename}>"
        )
        file.Close()
        return

    nbins = 30

    # ---------------------------------------------------------------------
    # Create reconstructed-energy histogram
    # ---------------------------------------------------------------------

    h = ROOT.TH1D(
        "h",
        "",
        nbins,
        xmin,
        xmax
    )

    # Process each event explicitly.
    #
    # This is slightly different from the C++ version, where the
    # processing functions were passed to TTree::Draw as a formula.
    # Doing it explicitly in Python is considerably easier and clearer.
    #
    # Get the branches through a Python loop.

    for event in tree:

        e_ecal = event.ECAL_eresponse
        e_hcal = event.HCAL_eresponse

        reco_energy = (
            processECAL(e_ecal)
            + processHCAL(e_hcal)
        )

        h.Fill(reco_energy)

    # ---------------------------------------------------------------------
    # Histogram settings
    # ---------------------------------------------------------------------

    h.GetXaxis().SetRangeUser(xmin, xmax)
    h.GetYaxis().SetLabelSize(0.05)
    h.SetMinimum(0.1)
    h.SetTitle("")
    h.Sumw2()

    # ---------------------------------------------------------------------
    # Plot
    # ---------------------------------------------------------------------

    ROOT.gStyle.SetOptStat(0)

    c1 = ROOT.TCanvas(
        f"c1_{pname}_{penergy_GeV}",
        "c1",
        10,
        31,
        525,
        500
    )

    h.Draw("E0")

    leg = ROOT.TLegend(
        0.6127591,
        0.7321938,
        0.9128024,
        0.94302,
        "",
        "brNDC"
    )

    leg.SetBorderSize(1)
    leg.SetLineColor(1)
    leg.SetLineStyle(1)
    leg.SetLineWidth(1)
    leg.SetFillColor(0)
    leg.SetFillStyle(1001)

    entry = leg.AddEntry(
        h,
        f"{pname} at {penergy_GeV:.1f} GeV",
        "lep"
    )

    entry.SetFillStyle(101)
    entry.SetLineColor(1)
    entry.SetLineStyle(1)
    entry.SetLineWidth(1)
    entry.SetMarkerColor(1)
    entry.SetMarkerStyle(1)
    entry.SetMarkerSize(1)
    entry.SetTextFont(42)

    leg.Draw()

    c1.cd()

    # ---------------------------------------------------------------------
    # Fit around maximum
    # ---------------------------------------------------------------------

    nBinsAroundMaximum = 5

    fitOK, hmean, hsigma = fitAroundMaximum(
        h,
        nBinsAroundMaximum
    )

    # ---------------------------------------------------------------------
    # Compute beam momentum
    # ---------------------------------------------------------------------

    if pname not in pname_pmassGeV:
        print(f"Unknown particle name: {pname}")
        file.Close()
        return

    pmomentum_GeV = ComputeMomentum(
        penergy_GeV,
        pname
    )

    # ---------------------------------------------------------------------
    # Write results
    # ---------------------------------------------------------------------

    if fitOK:

        response = hmean / pmomentum_GeV

        with open("centroids.txt", "a") as ofile:

            ofile.write(
                f"{pname}\t"
                f"{penergy_GeV}\t"
                f"{hmean}\t"
                f"{hsigma}\t"
                f"{pmomentum_GeV}\t"
                f"{response}\t"
                f"\n"
            )



    canvasFileName = (
        f"HCALTB2006_{pname}_{penergy_GeV:.6f}_"
        f"{nevents}evt_CMS_airECAL{airECAL}_reco.png"
    )

    c1.SaveAs(canvasFileName)

    file.Close()
    
if __name__ == "__main__":

    import argparse

    parser = argparse.ArgumentParser(
        description="Reconstruct and plot CALICE HCAL test beam energy"
    )

    parser.add_argument(
        "pname",
        help="Particle name"
    )

    parser.add_argument(
        "energy",
        type=float,
        help="Beam kinetic energy in GeV"
    )

    parser.add_argument(
        "xmin",
        type=float,
        help="Histogram minimum in GeV"
    )

    parser.add_argument(
        "xmax",
        type=float,
        help="Histogram maximum in GeV"
    )

    parser.add_argument(
        "--nevents",
        type=int,
        default=20000,
        help="Number of events (default: 20000)"
    )

    parser.add_argument(
        "--airECAL",
        type=int,
        default=0,
        help="Use air ECAL (default: 0)"
    )

    parser.add_argument(
        "--ECAL-calibrationFactor",
        type=float,
        default=1.01,
        help="ECAL calibration factor (default: 1.01)"
    )

    parser.add_argument(
        "--HCAL-calibrationFactor",
        type=float,
        default=106.5,
        help="HCAL calibration factor (default: 106.5)"
    )

    parser.add_argument(
        "--ECAL-smearing",
        type=float,
        default=0.362,
        help="ECAL smearing in GeV (default: 0.362)"
    )

    parser.add_argument(
        "--HCAL-smearing",
        type=float,
        default=0.64,
        help="HCAL smearing in GeV (default: 0.64)"
    )

    args = parser.parse_args()

    ECAL_calibrationFactor=args.ECAL_calibrationFactor
    HCAL_calibrationFactor=args.HCAL_calibrationFactor
    ECAL_smearing=args.ECAL_smearing
    HCAL_smearing=args.HCAL_smearing

    compute_response(
        pname=args.pname,
        penergy_GeV=args.energy,
        xmin=args.xmin,
        xmax=args.xmax,
        nevents=args.nevents,
        airECAL=args.airECAL
    )
