/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#include <slapsegiii.h>

SlapSegIII::Coordinate::Coordinate(
    const int32_t x,
    const int32_t y)
    noexcept :
    x{x},
    y{y}
{

}

SlapSegIII::SegmentationPosition::Result::Result(
    const Code code,
    const std::string &message) :
    code{code},
    message{message}
{

}

SlapSegIII::SegmentationPosition::SegmentationPosition()
{

}

SlapSegIII::SegmentationPosition::SegmentationPosition(
    const FrictionRidgeGeneralizedPosition frgp,
    const Coordinate &tl,
    const Coordinate &tr,
    const Coordinate &bl,
    const Coordinate &br,
    const Result result) :
    frgp{frgp},
    tl{tl},
    tr{tr},
    bl{bl},
    br{br},
    result{result}
{

}

SlapSegIII::SlapImage::SlapImage()
{

}

SlapSegIII::SlapImage::SlapImage(
    const uint16_t width,
    const uint16_t height,
    const uint16_t ppi,
    const Kind kind,
    const CaptureTechnology captureTechnology,
    const Orientation orientation,
    const std::vector<std::byte> &pixels) :
    width{width},
    height{height},
    ppi{ppi},
    kind{kind},
    captureTechnology{captureTechnology},
    orientation{orientation},
    pixels{pixels}
{

}

SlapSegIII::SubmissionIdentification::SubmissionIdentification()
{

}

SlapSegIII::SubmissionIdentification::SubmissionIdentification(
    const std::string &libraryIdentifier,
    const uint16_t version,
    const std::string &marketingIdentifier) :
    version(version),
    libraryIdentifier(libraryIdentifier),
    marketingIdentifier(marketingIdentifier)
{

}

SlapSegIII::ReturnStatus::ReturnStatus(
    const Code code,
    const std::set<SlapImage::Deficiency> &imageDeficiencies,
    const std::string &message) :
    code{code},
    imageDeficiencies{imageDeficiencies},
    message{message}
{

}

SlapSegIII::Interface::~Interface()
{

}
