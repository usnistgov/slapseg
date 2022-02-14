/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#define RANDOMLY_FAIL

#ifdef RANDOMLY_FAIL
#include <random>
#include <type_traits>
#endif

#include <slapsegiii_nullimpl.h>

SlapSegIII::SubmissionIdentification
SlapSegIII::NullImplementation::getIdentification()
    const
{
	static const uint16_t Version{0x0001};
	static const std::string LibraryIdentifier{"nullimpl"};
	static const std::string MarketingIdentifier{"NIST SlapSegIII Stub "
	    "Implementation (version 0.0.1)"};

	return {LibraryIdentifier, Version, MarketingIdentifier};
}

std::tuple<std::set<SlapSegIII::SlapImage::Kind>, bool>
SlapSegIII::NullImplementation::getSupported()
    const
{
	const std::set<SlapSegIII::SlapImage::Kind> kinds{
	    SlapImage::Kind::TwoInch, SlapImage::Kind::ThreeInch,
	    SlapImage::Kind::UpperPalm, SlapImage::Kind::FullPalm};
	return (std::make_tuple(kinds, false));
}

std::tuple<SlapSegIII::ReturnStatus,
    std::vector<SlapSegIII::SegmentationPosition>>
SlapSegIII::NullImplementation::segment(
    const SlapImage &image)
{
	std::vector<SlapSegIII::SegmentationPosition> positions{};
	const auto supportedSlaps = std::get<0>(this->getSupported());
	if (supportedSlaps.find(image.kind) == supportedSlaps.cend())
		return std::make_tuple(
		    ReturnStatus{ReturnStatus::Code::UnsupportedSlapType},
		    positions);

	positions.reserve(
	    image.orientation == SlapImage::Orientation::Thumbs ? 2 : 4);

	#ifdef RANDOMLY_FAIL
	static std::random_device rd{};
	static std::default_random_engine generator{rd()};
	static std::uniform_int_distribution<uint8_t> overallFail(1, 100);
	static std::uniform_int_distribution<uint8_t> positionFail(1, 4);

	const auto failCode = overallFail(generator);
	/* Randomly fail to process image */
	if (failCode == 1) {
		return (std::make_tuple(ReturnStatus(
		    ReturnStatus::Code::VendorDefined, {}, "Randomly failing"),
		    positions));
	/* Randomly fail to find a finger in an image */
	} else if (failCode == 50) {
		const auto posFailCode = positionFail(generator);

		SegmentationPosition failedPos{};
		failedPos.result = SegmentationPosition::Result(
		    SegmentationPosition::Result::Code::FingerNotFound,
		    "Randomly not finding finger");

		switch (image.orientation) {
		case SlapImage::Orientation::Thumbs:
			if (posFailCode == 1) {
				failedPos.frgp =
				    FrictionRidgeGeneralizedPosition::LeftThumb;
				positions.push_back(failedPos);
			} else {
				positions.emplace_back(
				    FrictionRidgeGeneralizedPosition::LeftThumb,
				    Coordinate{0, 0}, Coordinate{10, 0},
				    Coordinate{0, 10}, Coordinate{10, 10});
			}
			if (posFailCode == 2) {
				failedPos.frgp =
				    FrictionRidgeGeneralizedPosition::RightThumb;
				positions.push_back(failedPos);
			} else {
				positions.emplace_back(
				    FrictionRidgeGeneralizedPosition::RightThumb,
				    Coordinate{0, 0}, Coordinate{10, 0},
				    Coordinate{0, 10}, Coordinate{10, 10});
			}
			break;
		case SlapImage::Orientation::Right:
			if (posFailCode == 1) {
				failedPos.frgp =
				    FrictionRidgeGeneralizedPosition::RightIndex;
				positions.push_back(failedPos);
			} else {
				positions.emplace_back(
				    FrictionRidgeGeneralizedPosition::RightIndex,
				    Coordinate{0, 0}, Coordinate{10, 0},
				    Coordinate{0, 10}, Coordinate{10, 10});
			}
			if (posFailCode == 2) {
				failedPos.frgp =
				    FrictionRidgeGeneralizedPosition::
				    RightMiddle;
				positions.push_back(failedPos);
			} else {
				positions.emplace_back(
				    FrictionRidgeGeneralizedPosition::
				    RightMiddle, Coordinate{0, 0},
				    Coordinate{10, 0}, Coordinate{0, 10},
				    Coordinate{10, 10});
			}
			if (posFailCode == 3) {
				failedPos.frgp =
				    FrictionRidgeGeneralizedPosition::RightRing;
				positions.push_back(failedPos);
			} else {
				positions.emplace_back(
				    FrictionRidgeGeneralizedPosition::RightRing,
				    Coordinate{0, 0}, Coordinate{10, 0},
				    Coordinate{0, 10}, Coordinate{10, 10});
			}
			if (posFailCode == 4) {
				failedPos.frgp =
				    FrictionRidgeGeneralizedPosition::
				    RightLittle;
				positions.push_back(failedPos);
			} else {
				positions.emplace_back(
				    FrictionRidgeGeneralizedPosition::
				    RightLittle, Coordinate{0, 0},
				    Coordinate{10, 0}, Coordinate{0, 10},
				    Coordinate{10, 10});
			}
			break;
		case SlapImage::Orientation::Left:
			if (posFailCode == 1) {
				failedPos.frgp =
				    FrictionRidgeGeneralizedPosition::LeftIndex;
				positions.push_back(failedPos);
			} else {
				positions.emplace_back(
				    FrictionRidgeGeneralizedPosition::LeftIndex,
				    Coordinate{0, 0}, Coordinate{10, 0},
				    Coordinate{0, 10}, Coordinate{10, 10});
			}
			if (posFailCode == 2) {
				failedPos.frgp =
				    FrictionRidgeGeneralizedPosition::
				    LeftMiddle;
				positions.push_back(failedPos);
			} else {
				positions.emplace_back(
				    FrictionRidgeGeneralizedPosition::
				    LeftMiddle, Coordinate{0, 0},
				    Coordinate{10, 0}, Coordinate{0, 10},
				    Coordinate{10, 10});
			}
			if (posFailCode == 3) {
				failedPos.frgp =
				    FrictionRidgeGeneralizedPosition::LeftRing;
				positions.push_back(failedPos);
			} else {
				positions.emplace_back(
				    FrictionRidgeGeneralizedPosition::LeftRing,
				    Coordinate{0, 0}, Coordinate{10, 0},
				    Coordinate{0, 10}, Coordinate{10, 10});
			}
			if (posFailCode == 4) {
				failedPos.frgp =
				    FrictionRidgeGeneralizedPosition::
				    LeftLittle;
				positions.push_back(failedPos);
			} else {
				positions.emplace_back(
				    FrictionRidgeGeneralizedPosition::
				    LeftLittle, Coordinate{0, 0},
				    Coordinate{10, 0}, Coordinate{0, 10},
				    Coordinate{10, 10});
			}
			break;
		}

		return (std::make_tuple(ReturnStatus(
		    ReturnStatus::Code::RequestRecaptureWithAttempt,
		    {SlapImage::Deficiency::HandGeometry}, "Randomly failing"),
		    positions));
	}
	#endif

	switch (image.orientation) {
	case SlapImage::Orientation::Thumbs:
		positions.emplace_back(
		    FrictionRidgeGeneralizedPosition::LeftThumb,
		    Coordinate{0, 0}, Coordinate{10, 0}, Coordinate{0, 10},
		    Coordinate{10, 10});
		positions.emplace_back(
		    FrictionRidgeGeneralizedPosition::RightThumb,
		    Coordinate{0, 0}, Coordinate{10, 0}, Coordinate{0, 10},
		    Coordinate{10, 10});
		break;
	case SlapImage::Orientation::Right:
		positions.emplace_back(
		    FrictionRidgeGeneralizedPosition::RightIndex,
		    Coordinate{0, 0}, Coordinate{10, 0}, Coordinate{0, 10},
		    Coordinate{10, 10});
		positions.emplace_back(
		    FrictionRidgeGeneralizedPosition::RightMiddle,
		    Coordinate{0, 0}, Coordinate{10, 0}, Coordinate{0, 10},
		    Coordinate{10, 10});
		positions.emplace_back(
		    FrictionRidgeGeneralizedPosition::RightRing,
		    Coordinate{0, 0}, Coordinate{10, 0}, Coordinate{0, 10},
		    Coordinate{10, 10});
		positions.emplace_back(
		    FrictionRidgeGeneralizedPosition::RightLittle,
		    Coordinate{0, 0}, Coordinate{10, 0}, Coordinate{0, 10},
		    Coordinate{10, 10});
		break;
	case SlapImage::Orientation::Left:
		positions.emplace_back(
		    FrictionRidgeGeneralizedPosition::LeftIndex,
		    Coordinate{0, 0}, Coordinate{10, 0}, Coordinate{0, 10},
		    Coordinate{10, 10});
		positions.emplace_back(
		    FrictionRidgeGeneralizedPosition::LeftMiddle,
		    Coordinate{0, 0}, Coordinate{10, 0}, Coordinate{0, 10},
		    Coordinate{10, 10});
		positions.emplace_back(
		    FrictionRidgeGeneralizedPosition::LeftRing,
		    Coordinate{0, 0}, Coordinate{10, 0}, Coordinate{0, 10},
		    Coordinate{10, 10});
		positions.emplace_back(
		    FrictionRidgeGeneralizedPosition::LeftLittle,
		    Coordinate{0, 0}, Coordinate{10, 0}, Coordinate{0, 10},
		    Coordinate{10, 10});
		break;
	}

	return std::make_tuple(ReturnStatus{}, positions);
}

std::tuple<SlapSegIII::ReturnStatus, SlapSegIII::SlapImage::Orientation>
SlapSegIII::NullImplementation::determineOrientation(
    const SlapSegIII::SlapImage &image)
{
	return (std::make_tuple(ReturnStatus::Code::NotImplemented,
	    SlapImage::Orientation{}));
}

/******************************************************************************/

SlapSegIII::NullImplementation::NullImplementation(
    const std::filesystem::path &configurationDirectory) :
    SlapSegIII::Interface{},
    configurationDirectory{configurationDirectory}
{
	/* Parse contents of configurationDirectory, if necessary */
}

std::shared_ptr<SlapSegIII::Interface>
SlapSegIII::Interface::getImplementation(
    const std::filesystem::path &configurationDirectory)
{
	return (std::make_shared<SlapSegIII::NullImplementation>(
	    configurationDirectory));
}

