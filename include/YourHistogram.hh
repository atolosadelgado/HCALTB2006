#ifndef YOURHISTOGRAM
#define YOURHISTOGRAM

#include <vector>
#include <stdexcept>
#include <cmath>

#include "TH1.h"


class YourHistogram {
private:
    int nbins_;
    double xmin_;
    double xmax_;
    double binWidth_;
    std::vector<int> bins_;

public:
    YourHistogram(int nbins, double xmin, double xmax)
        : nbins_(nbins), xmin_(xmin), xmax_(xmax), bins_(nbins, 0)
    {
        if (nbins <= 0) {
            throw std::invalid_argument("Number of bins must be positive.");
        }
        if (xmax <= xmin) {
            throw std::invalid_argument("xmax must be greater than xmin.");
        }
        binWidth_ = (xmax_ - xmin_) / nbins_;
    }

    void fill(double x) {
        if (x < xmin_ || x >= xmax_) {
            // Ignore values outside the histogram range
            return;
        }
        int index = static_cast<int>((x - xmin_) / binWidth_);
        if (index >= 0 && index < nbins_) {
            bins_[index]++;
        }
    }

    int getBinContent(int bin) const {
        if (bin < 0 || bin >= nbins_) {
            throw std::out_of_range("Bin index out of range.");
        }
        return bins_[bin];
    }

    // add another histogram to this one
    void add(const YourHistogram& other) {
        if (nbins_ != other.nbins_ ||
            xmin_ != other.xmin_ ||
            xmax_ != other.xmax_)
        {
            throw std::invalid_argument("Histograms must have identical binning.");
        }

        for (int i = 0; i < nbins_; ++i) {
            bins_[i] += other.bins_[i];
        }
    }

    double mean() const {
        double sumW = 0.0;
        double sumWX = 0.0;

        for (int i = 0; i < nbins_; ++i) {
            double c = bins_[i];
            if (c == 0) continue;

            double center = xmin_ + (i + 0.5) * binWidth_;

            sumW  += c;
            sumWX += c * center;
        }

        if (sumW == 0) {
            throw std::runtime_error("Cannot compute mean of empty histogram.");
        }

        return sumWX / sumW;
    }

    double meanError() const {
        double sumW = 0.0;
        double sumWX = 0.0;

        // First compute the mean
        for (int i = 0; i < nbins_; ++i) {
            double c = bins_[i];
            if (c == 0) continue;

            double center = xmin_ + (i + 0.5) * binWidth_;
            sumW  += c;
            sumWX += c * center;
        }

        if (sumW == 0) {
            throw std::runtime_error("Cannot compute mean error of empty histogram.");
        }

        double mu = sumWX / sumW;

        // Now compute the variance
        double sumVar = 0.0;
        for (int i = 0; i < nbins_; ++i) {
            double c = bins_[i];
            if (c == 0) continue;

            double center = xmin_ + (i + 0.5) * binWidth_;
            double diff = center - mu;

            sumVar += c * diff * diff;
        }

        double variance = sumVar / sumW;
        double sigma = std::sqrt(variance);

        // Standard error of the mean
        return sigma / std::sqrt(sumW);
    }

    void FillTH1(TH1 * h)
    {
        for (int i = 0; i < nbins_; ++i) {
            h->SetBinContent(i + 1, bins_[i]);
        }
    }


    int getNBins() const { return nbins_; }
    double getXMin() const { return xmin_; }
    double getXMax() const { return xmax_; }
    double getBinWidth() const { return binWidth_; }


};

#endif
