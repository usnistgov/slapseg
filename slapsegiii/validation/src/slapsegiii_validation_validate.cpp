/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties.  Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain.  NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#include <slapsegiii_validation_validate.h>

SlapSegIII::Validation::Validate::Errors
SlapSegIII::Validation::Validate::validateSegmentationPosition(
    const SegmentationPosition &position,
    std::shared_ptr<SlapImage> slapImage)
{
	Errors errors{};

	if (hasIrregularCoordinates(position))
		errors.set(
		    static_cast<typename std::underlying_type<ErrorCode>::type>(
		    ErrorCode::IrregularCoordinates));
	if (!isRectangular(position))
		errors.set(
		    static_cast<typename std::underlying_type<ErrorCode>::type>(
		    ErrorCode::NonRectangularCoordinates));

	if (!canBeRotated(slapImage->kind)) {
		if (isRotated(position))
			errors.set(
			    static_cast<typename
			        std::underlying_type<ErrorCode>::type>(
			        ErrorCode::Rotated));
		if (isOutsideImage(position, slapImage->width,
		    slapImage->height))
			errors.set(static_cast<typename
			    std::underlying_type<ErrorCode>::type>(
			    ErrorCode::CoordinatesOutsideImage));
	}

	return (errors);
}

SlapSegIII::Validation::Validate::Deficiencies
SlapSegIII::Validation::Validate::gatherDeficiencies(
    const ReturnStatus &rs)
{
	Deficiencies deficiencies{};

	if (rs.imageDeficiencies.find(SlapImage::Deficiency::Artifacts) !=
	    rs.imageDeficiencies.end())
		deficiencies.set(static_cast<typename std::underlying_type<
		    SlapImage::Deficiency>::type>(
		    SlapImage::Deficiency::Artifacts));

	if (rs.imageDeficiencies.find(SlapImage::Deficiency::ImageQuality) !=
	    rs.imageDeficiencies.end())
		deficiencies.set(static_cast<typename std::underlying_type<
		    SlapImage::Deficiency>::type>(
		    SlapImage::Deficiency::ImageQuality));

	if (rs.imageDeficiencies.find(SlapImage::Deficiency::HandGeometry) !=
	    rs.imageDeficiencies.end())
		deficiencies.set(static_cast<typename std::underlying_type<
		    SlapImage::Deficiency>::type>(
		    SlapImage::Deficiency::HandGeometry));

	if (rs.imageDeficiencies.find(SlapImage::Deficiency::Incomplete) !=
	    rs.imageDeficiencies.end())
		deficiencies.set(static_cast<typename std::underlying_type<
		    SlapImage::Deficiency>::type>(
		    SlapImage::Deficiency::Incomplete));

	return (deficiencies);
}

bool
SlapSegIII::Validation::Validate::canBeRotated(
    const SlapImage::Kind kind)
{
	return (kind != SlapImage::Kind::ThreeInch);
}

bool
SlapSegIII::Validation::Validate::hasCorrectQuantity(
    const std::vector<SegmentationPosition> &positions,
    const SlapImage::Orientation orientation)
{
	return (positions.size() == getCorrectQuantity(orientation));
}

uint8_t
SlapSegIII::Validation::Validate::getCorrectQuantity(
    const SlapImage::Orientation orientation)
{
	switch (orientation) {
	case SlapImage::Orientation::Thumbs:
		return (2);
	case SlapImage::Orientation::Left:
		/* FALLTHROUGH */
	case SlapImage::Orientation::Right:
		return (4);
	}

	/* Not reached */
	return (0);
}

std::set<SlapSegIII::FrictionRidgeGeneralizedPosition>
SlapSegIII::Validation::Validate::getExpectedFrictionRidgeGeneralizedPositions(
    const SlapImage::Orientation orientation)
{
	switch (orientation) {
	case SlapImage::Orientation::Thumbs:
		return {FrictionRidgeGeneralizedPosition::LeftThumb,
		    FrictionRidgeGeneralizedPosition::RightThumb};
	case SlapImage::Orientation::Left:
		return {FrictionRidgeGeneralizedPosition::LeftIndex,
		    FrictionRidgeGeneralizedPosition::LeftMiddle,
		    FrictionRidgeGeneralizedPosition::LeftRing,
		    FrictionRidgeGeneralizedPosition::LeftLittle};
	case SlapImage::Orientation::Right:
		return {FrictionRidgeGeneralizedPosition::RightIndex,
		    FrictionRidgeGeneralizedPosition::RightMiddle,
		    FrictionRidgeGeneralizedPosition::RightRing,
		    FrictionRidgeGeneralizedPosition::RightLittle};
	}

	/* Not reached */
	return {};
}

bool
SlapSegIII::Validation::Validate::isOutsideImage(
    const SegmentationPosition &p,
    const uint16_t width,
    const uint16_t height)
{
	return (
	    (p.tl.x < 0) || (p.tl.y < 0) ||
	    (p.tr.x < 0) || (p.tr.y < 0) ||
	    (p.bl.x < 0) || (p.bl.y < 0) ||
	    (p.br.x < 0) || (p.br.y < 0) ||
	    (p.tl.x >= width) || (p.tl.y >= height) ||
	    (p.tr.x >= width) || (p.tr.y >= height) ||
	    (p.bl.x >= width) || (p.bl.y >= height) ||
	    (p.br.x >= width) || (p.br.y >= height));
}

bool
SlapSegIII::Validation::Validate::isRectangular(
    const SegmentationPosition &p)
{
	const auto topLen = ((p.tr.x - p.tl.x) * (p.tr.x - p.tl.x)) +
	    ((p.tr.y - p.tl.y) * (p.tr.y - p.tl.y));
	const auto bottomLen = ((p.br.x - p.bl.x) * (p.br.x - p.bl.x)) +
	    ((p.br.y - p.bl.y) * (p.br.y - p.bl.y));

	if (topLen != bottomLen)
		return (false);

	const auto leftLen = ((p.tl.x - p.bl.x) * (p.tl.x - p.bl.x)) +
	    ((p.tl.y - p.bl.y) * (p.tl.y - p.bl.y));
	const auto rightLen = ((p.tr.x - p.br.x) * (p.tr.x - p.br.x)) +
	    ((p.tr.y - p.br.y) * (p.tr.y - p.br.y));

	return (leftLen == rightLen);
}

bool
SlapSegIII::Validation::Validate::isRotated(
    const SegmentationPosition &p)
{
	return (p.tl.x != p.bl.x);
}

bool
SlapSegIII::Validation::Validate::hasIrregularCoordinates(
    const SegmentationPosition &p)
{
	return (!(
	    /* Left are left of right */
	    (p.tl.x < p.tr.x) &&
	    (p.bl.x < p.br.x) &&
	    /* Top are above bottom */
	    (p.tl.y < p.bl.y) &&
	    (p.tr.y < p.br.y)));
}
