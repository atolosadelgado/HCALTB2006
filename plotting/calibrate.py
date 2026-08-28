import math
import sys

from array import array

import ROOT


# ============================================================================
# Particle masses
# ============================================================================

pname_pmassGeV = {
    "pi-": 0.1396,
    "pi+": 0.1396,
    "proton": 0.9383,
    "anti_proton": 0.9383,
    "kaon+": 0.4937,
    "kaon-": 0.4937,
    "e-": 0.000511,
}


def compute_momentum(ekin_GeV, pname):
    mass_GeV = pname_pmassGeV[pname]

    return math.sqrt(
        ekin_GeV * (ekin_GeV + 2.0 * mass_GeV)
    )


# ============================================================================
# Plot calibration results
# ============================================================================

def plot_calibration(filename, detector):

    try:
        with open(filename, "r") as file:
            energy = []
            response = []

            for line in file:

                line = line.strip()

                if not line:
                    continue

                if line.startswith("#"):
                    continue

                fields = line.split()

                if len(fields) >= 3:
                    pname = fields[0]
                    kinetic_energy = float(fields[1])
                    mean_response = float(fields[2])

                    energy.append(kinetic_energy)
                    response.append(mean_response)

    except OSError:
        print(f"Could not open <{filename}>", file=sys.stderr)
        return

    if not energy:
        print(f"No data found in <{filename}>", file=sys.stderr)
        return


    # ------------------------------------------------------------------------
    # Graph
    # ------------------------------------------------------------------------

    graph = ROOT.TGraph(
        len(energy),
        array("d", energy),
        array("d", response)
    )

    graph.SetTitle(f"{detector} calibration")
    graph.GetXaxis().SetTitle("Kinetic energy [GeV]")
    graph.GetYaxis().SetTitle("1/response")

    graph.SetMarkerStyle(20)
    graph.SetMarkerSize(1.1)
    graph.SetLineWidth(2)


    # ------------------------------------------------------------------------
    # Canvas
    # ------------------------------------------------------------------------

    canvas = ROOT.TCanvas(
        f"c{detector}Calibration",
        f"{detector} calibration",
        800,
        600
    )

    canvas.SetGrid()
    canvas.SetLogx()

    graph.Draw("APL")


    # ------------------------------------------------------------------------
    # Response = 1 reference line
    # ------------------------------------------------------------------------

    xmax = 1.05 * max(energy)

    reference_line = ROOT.TLine(
        1.0,
        1.0,
        xmax,
        1.0
    )

    reference_line.SetLineStyle(2)
    reference_line.Draw()


    # ------------------------------------------------------------------------
    # Save
    # ------------------------------------------------------------------------

    canvas.SaveAs(f"{detector}_calibration.png")
    canvas.SaveAs(f"{detector}_calibration.root")


# ============================================================================
# Calibration
# ============================================================================

def calibrate(
    detector="ECAL",
    scanFile="calibration.scan",
    nevents=2000,
    doPlot=True
):

    ROOT.gROOT.SetBatch(True)
    ROOT.gStyle.SetOptStat(0)


    # ------------------------------------------------------------------------
    # Detector configuration
    # ------------------------------------------------------------------------

    if detector == "ECAL":
        responseBranch = "ECAL_eresponse"
        airECAL = 0

    elif detector == "HCAL":
        responseBranch = "HCAL_eresponse"
        airECAL = 1

    else:
        print(
            f"Unknown detector <{detector}>. "
            "Use ECAL or HCAL.",
            file=sys.stderr
        )
        return


    # ------------------------------------------------------------------------
    # Input scan
    # ------------------------------------------------------------------------

    try:
        input_file = open(scanFile, "r")
    except OSError:
        print(
            f"Could not open <{scanFile}>",
            file=sys.stderr
        )
        return


    # ------------------------------------------------------------------------
    # Output file
    # ------------------------------------------------------------------------

    outputFilename = f"{detector}_calibration.txt"

    try:
        output = open(outputFilename, "w")
    except OSError:
        print(
            f"Could not open <{outputFilename}>",
            file=sys.stderr
        )
        input_file.close()
        return

    output.write(
        f"# particle\tkinetic_energy_GeV\t"
        f"mean_{responseBranch}/kinetic_energy_GeV\n"
    )


    # ------------------------------------------------------------------------
    # Loop over scan
    # ------------------------------------------------------------------------

    for line in input_file:

        line = line.strip()

        if not line or line.startswith("#"):
            continue

        fields = line.split()

        if len(fields) < 2:
            continue

        pname = fields[0]
        energy = float(fields[1])


        filename = (
            f"HCALTB2006_{pname}_{energy:.6f}_"
            f"{nevents}evt_CMS_airECAL{airECAL}.root"
        )

        print(f"Processing {filename}")


        # --------------------------------------------------------------------
        # Open ROOT file
        # --------------------------------------------------------------------

        file = ROOT.TFile.Open(filename)

        if not file or file.IsZombie():
            print(
                f"Could not open <{filename}>",
                file=sys.stderr
            )

            if file:
                file.Close()

            continue


        # --------------------------------------------------------------------
        # Get tree
        # --------------------------------------------------------------------

        tree = file.Get("tree")

        if not tree:
            print(
                f"Could not find tree in <{filename}>",
                file=sys.stderr
            )

            file.Close()
            continue


        # --------------------------------------------------------------------
        # Read response branch
        # --------------------------------------------------------------------

        histogram_name = "h"

        tree.Draw(
            f"{responseBranch}>>{histogram_name}",
            "",
            "goff"
        )

        h = ROOT.gDirectory.Get(histogram_name)

        if not h:
            print(
                f"Could not create histogram for "
                f"{responseBranch} in <{filename}>",
                file=sys.stderr
            )

            file.Close()
            continue

        mean = h.GetMean()

        MeV_to_GeV = 0.001

        momentum = compute_momentum(
            energy,
            pname
        )

        response = momentum / (mean * MeV_to_GeV)


        # --------------------------------------------------------------------
        # Save result
        # --------------------------------------------------------------------

        output.write(
            f"{pname}\t"
            f"{energy}\t"
            f"{response}\n"
        )

        print(
            f"  mean {responseBranch} = {response}"
        )


        file.Close()


    input_file.close()
    output.close()


    # ------------------------------------------------------------------------
    # Plot results
    # ------------------------------------------------------------------------

    if doPlot:
        plot_calibration(
            outputFilename,
            detector
        )


# ============================================================================
# Main
# ============================================================================

import argparse

if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="Calibrate ECAL or HCAL response"
    )

    parser.add_argument(
        "--detector",
        choices=["ECAL", "HCAL"],
        default="ECAL",
        help="Detector to calibrate (default: ECAL)"
    )

    parser.add_argument(
        "--scan-file",
        default="calibration.scan",
        help="Input scan file (default: calibration.scan)"
    )

    parser.add_argument(
        "--nevents",
        type=int,
        default=2000,
        help="Number of events per ROOT file (default: 2000)"
    )

    parser.add_argument(
        "--no-plot",
        action="store_true",
        help="Do not produce calibration plot"
    )

    args = parser.parse_args()

    calibrate(
        detector=args.detector,
        scanFile=args.scan_file,
        nevents=args.nevents,
        doPlot=not args.no_plot
    )


