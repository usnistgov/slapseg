/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#include <slapsegiii_nullimpl.h>

std::tuple<std::string, uint16_t>
SlapSegIII::NullImplementation::getIdentification()
    const
{
	static const uint16_t Version{0x0001};
	static const std::string Identifier{"nullimpl"};

	return (std::make_tuple(Identifier, Version));
}


std::set<SlapSegIII::SlapImage::Kind>
SlapSegIII::NullImplementation::getSupported()
    const
{
	return {SlapImage::Kind::TwoInch, SlapImage::Kind::ThreeInch,
	    SlapImage::Kind::UpperPalm, SlapImage::Kind::FullPalm};
}

std::tuple<SlapSegIII::ReturnStatus,
    std::vector<SlapSegIII::SegmentationPosition>>
SlapSegIII::NullImplementation::segment(
    const SlapImage &image)
{
	std::vector<SlapSegIII::SegmentationPosition> positions{};
	const auto supportedSlaps = this->getSupported();
	if (supportedSlaps.find(image.kind) == supportedSlaps.cend())
		return std::make_tuple(
		    ReturnStatus{ReturnStatus::Code::UnsupportedSlapType},
		    positions);

	positions.reserve(
	    image.orientation == SlapImage::Orientation::Thumbs ? 2 : 4);

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
		if (image.kind == SlapImage::Kind::FullPalm)
			positions.emplace_back(
			    FrictionRidgeGeneralizedPosition::RightLittle,
			    Coordinate{0, 0}, Coordinate{10, 0},
			    Coordinate{0, 10}, Coordinate{10, 10});
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
		if (image.kind == SlapImage::Kind::FullPalm)
			positions.emplace_back(
			    FrictionRidgeGeneralizedPosition::LeftThumb,
			    Coordinate{0, 0}, Coordinate{10, 0},
			    Coordinate{0, 10}, Coordinate{10, 10});
		break;
	}

	return std::make_tuple(ReturnStatus{}, positions);
}

std::shared_ptr<SlapSegIII::Interface>
SlapSegIII::Interface::getImplementation()
{
	return (std::make_shared<SlapSegIII::NullImplementation>());
}

