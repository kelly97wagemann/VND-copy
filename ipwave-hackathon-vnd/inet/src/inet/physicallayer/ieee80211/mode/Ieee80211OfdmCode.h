//
// Copyright (C) 2014 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_IEEE80211OFDMCODE_H
#define __INET_IEEE80211OFDMCODE_H

#include "inet/physicallayer/common/bitlevel/AdditiveScrambling.h"
#include "inet/physicallayer/ieee80211/bitlevel/Ieee80211OfdmInterleaving.h"
#include "inet/physicallayer/ieee80211/bitlevel/Ieee80211ConvolutionalCode.h"
#include "inet/physicallayer/contract/bitlevel/ICode.h"

namespace inet {
namespace physicallayer {

class INET_API Ieee80211OfdmCode : public ICode
{
    protected:
        const ConvolutionalCode *convolutionalCode;
        const Ieee80211OfdmInterleaving *interleaving;
        const AdditiveScrambling *scrambling;

    public:
        Ieee80211OfdmCode(const ConvolutionalCode *convolutionalCode, const Ieee80211OfdmInterleaving *interleaving, const AdditiveScrambling *scrambling);

        virtual std::ostream& printToStream(std::ostream& stream, int level) const override;

        const ConvolutionalCode *getConvolutionalCode() const { return convolutionalCode; }
        const Ieee80211OfdmInterleaving *getInterleaving() const { return interleaving; }
        const AdditiveScrambling *getScrambling() const { return scrambling; }
};

class INET_API Ieee80211OfdmCompliantCodes
{
  public:
    // Convolutional codes supported by the OFDM PHY: 18.3.5.6 Convolutional encoder
    static const Ieee80211ConvolutionalCode ofdmConvolutionalCode1_2;
    static const Ieee80211ConvolutionalCode ofdmConvolutionalCode2_3;
    static const Ieee80211ConvolutionalCode ofdmConvolutionalCode3_4;
    static const Ieee80211ConvolutionalCode ofdmConvolutionalCode5_6;

    // Interleavings supported by the OFDM PHY: 18.3.5.7 Data interleaving
    static const Ieee80211OfdmInterleaving ofdmBPSKInterleaving;
    static const Ieee80211OfdmInterleaving ofdmQPSKInterleaving;
    static const Ieee80211OfdmInterleaving ofdmQAM16Interleaving;
    static const Ieee80211OfdmInterleaving ofdmQAM64Interleaving;
    static const Ieee80211OfdmInterleaving ofdmQAM256Interleaving;
    static const Ieee80211OfdmInterleaving ofdmQAM1024Interleaving;

    // Scrambling supported by the OFDM PHY: 18.3.5.5 PLCP DATA scrambler and descrambler
    static const AdditiveScrambling ofdmScrambling;

    // Codes: Table 18-4???Modulation-dependent parameters
    static const Ieee80211OfdmCode ofdmCC1_2BPSKInterleaving;
    static const Ieee80211OfdmCode ofdmCC1_2BPSKInterleavingWithoutScrambling; // For the SIGNAL field
    static const Ieee80211OfdmCode ofdmCC3_4BPSKInterleaving;
    static const Ieee80211OfdmCode ofdmCC1_2QPSKInterleaving;
    static const Ieee80211OfdmCode ofdmCC3_4QPSKInterleaving;
    static const Ieee80211OfdmCode ofdmCC1_2QAM16Interleaving;
    static const Ieee80211OfdmCode ofdmCC3_4QAM16Interleaving;
    static const Ieee80211OfdmCode ofdmCC2_3QAM64Interleaving;
    static const Ieee80211OfdmCode ofdmCC3_4QAM64Interleaving;
    static const Ieee80211OfdmCode ofdmCC5_6QAM64Interleaving;
    static const Ieee80211OfdmCode ofdmCC3_4QAM256Interleaving;
    static const Ieee80211OfdmCode ofdmCC5_6QAM256Interleaving;
    static const Ieee80211OfdmCode ofdmCC3_4QAM1024Interleaving;
    static const Ieee80211OfdmCode ofdmCC5_6QAM1024Interleaving;
};

} /* namespace physicallayer */
} /* namespace inet */

#endif /* __INET_IEEE80211OFDMCODE_H */
