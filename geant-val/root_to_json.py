#!/usr/bin/env python3

import argparse
import json
import ROOT


def graph_to_json(
    input_file,
    graph_name,
    output_file,
    particle,
    physics_list,
    geant4_version,
    test_name="CALICE",
    target_name="CALICE",
    observable_name=None,
):
    # Open ROOT file
    root_file = ROOT.TFile.Open(input_file, "READ")

    if not root_file or root_file.IsZombie():
        raise RuntimeError(f"Cannot open ROOT file: {input_file}")

    # Get TGraph
    graph = root_file.Get(graph_name)

    if not graph:
        raise RuntimeError(
            f"Graph '{graph_name}' not found in {input_file}"
        )

    if not graph.InheritsFrom("TGraph"):
        raise TypeError(
            f"Object '{graph_name}' is not a TGraph"
        )

    # Extract graph points
    n_points = graph.GetN()

    x_values = [float(graph.GetPointX(i)) for i in range(n_points)]
    y_values = [float(graph.GetPointY(i)) for i in range(n_points)]

    # Axis titles
    x_axis_name = ""
    y_axis_name = ""

    if graph.GetXaxis():
        x_axis_name = graph.GetXaxis().GetTitle()

    if graph.GetYaxis():
        y_axis_name = graph.GetYaxis().GetTitle()

    if observable_name is None:
        observable_name = graph_name

    # Geant-Val JSON
    data = {
        "article": {
            "inspireId": -1
        },

        "mctool": {
            "name": "GEANT4",
            "version": geant4_version,
            "model": physics_list
        },

        "testName": test_name,

        "metadata": {
            "observableName": observable_name,
            "reaction": "reaction name",
            "targetName": target_name,
            "beamParticle": particle,
            "beamEnergies": x_values,
            "secondaryParticle": "",
            "parameters": []
        },

        "plotType": "chart",

        "chart": {
            "xValues": x_values,
            "yValues": y_values,
            "xAxisName": x_axis_name,
            "yAxisName": y_axis_name,
            "yStatErrorsMinus": [0.0] * n_points,
            "yStatErrorsPlus": [0.0] * n_points,
            "ySysErrorsMinus": [],
            "ySysErrorsPlus": []
        }
    }

    # Write JSON
    with open(output_file, "w") as f:
        json.dump(data, f, indent=2)

    root_file.Close()

    print(f"Created Geant-Val JSON: {output_file}")


def main():

    parser = argparse.ArgumentParser(
        description="Convert a ROOT TGraph to Geant-Val JSON"
    )

    parser.add_argument(
        "--input",
        required=True,
        help="Input ROOT file"
    )

    parser.add_argument(
        "--graph",
        required=True,
        help="Name of the TGraph"
    )

    parser.add_argument(
        "--output",
        required=True,
        help="Output JSON file"
    )

    parser.add_argument(
        "--particle",
        required=True,
        help="Beam particle"
    )

    parser.add_argument(
        "--physics-list",
        required=True,
        help="Geant4 physics list"
    )

    parser.add_argument(
        "--geant4-version",
        required=True,
        help="Geant4 version"
    )

    parser.add_argument(
        "--test-name",
        default="Combined CMS ECAL-HCAL",
        help="Geant-Val test name"
    )

    parser.add_argument(
        "--target-name",
        default="Combined CMS ECAL-HCAL",
        help="Target/detector name"
    )

    parser.add_argument(
        "--observable-name",
        default=None,
        help="Observable name"
    )

    args = parser.parse_args()

    graph_to_json(
        input_file=args.input,
        graph_name=args.graph,
        output_file=args.output,
        particle=args.particle,
        physics_list=args.physics_list,
        geant4_version=args.geant4_version,
        test_name=args.test_name,
        target_name=args.target_name,
        observable_name=args.observable_name,
    )


if __name__ == "__main__":
    main()
