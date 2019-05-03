/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties.  Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain.  NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#include <cmath>

#include <slapsegiii_validation_validate.h>

SlapSegIII::Validation::Validate::Errors
SlapSegIII::Validation::Validate::validateSegmentationPosition(
    const SegmentationPosition &position,
    std::shared_ptr<SlapImage> slapImage)
{
	/* Don't validate coordinates if position was not set. */
	if (position.result.code != SegmentationPosition::Result::Code::Success)
		return (Errors{});

	Errors errors{};

	if (hasIrregularCoordinates(position))
		errors.set(
		    static_cast<typename std::underlying_type<ErrorCode>::type>(
		    ErrorCode::IrregularCoordinates));
	if (!isRectangular(position, slapImage->kind))
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
	return (getExpectedFrictionRidgeGeneralizedPositions(
	    orientation).size());
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
    const SegmentationPosition &p,
    const SlapImage::Kind kind)
{
	const int64_t topLen{((p.tr.x - p.tl.x) * (p.tr.x - p.tl.x)) +
	    ((p.tr.y - p.tl.y) * (p.tr.y - p.tl.y))};
	const int64_t bottomLen{((p.br.x - p.bl.x) * (p.br.x - p.bl.x)) +
	    ((p.br.y - p.bl.y) * (p.br.y - p.bl.y))};
	const int64_t leftLen{((p.tl.x - p.bl.x) * (p.tl.x - p.bl.x)) +
	    ((p.tl.y - p.bl.y) * (p.tl.y - p.bl.y))};
	const int64_t rightLen{((p.tr.x - p.br.x) * (p.tr.x - p.br.x)) +
	    ((p.tr.y - p.br.y) * (p.tr.y - p.br.y))};

	if (!canBeRotated(kind))
		/* Perfect rectangle */
		return ((topLen == bottomLen) && (leftLen == rightLen));

	/*
	 * Implementations might store coordinates as floating point, which
	 * could produce slightly imperfect rectangles due to loss of precision
	 * when converting to integer coordinates.
	 *
	 * We'll allow a certain tolerance for rotated segmentation positions.
	 */

	static const double angleTolerance{0.3};
	static const double rad2deg{180.0 / M_PI};

	/* Compute precise side lengths */
	const double realTopLen{std::sqrt(topLen)};
	const double realBottomLen{std::sqrt(bottomLen)};
	const double realLeftLen{std::sqrt(leftLen)};
	const double realRightLen{std::sqrt(rightLen)};

	/* Determine angles of the two triangles comprising the quadrilateral */
	const double t1a1{std::atan2(realBottomLen, realLeftLen) * rad2deg};
	const double t1a2{std::atan2(realLeftLen, realBottomLen) * rad2deg};
	const double t2a1{std::atan2(realRightLen, realTopLen) * rad2deg};
	const double t2a2{std::atan2(realTopLen, realRightLen) * rad2deg};

	/* The four angles of the segmentation position quadrilateral */
	const double a1{t1a1 + t2a1};
	const double a2{t1a2 + t2a2};
	const double a3{180.0 - t1a1 - t1a2};
	const double a4{180.0 - t2a1 - t2a2};

	/* Let no angle be more than angleTolerance different than perfect */
	return (
	    (std::abs(90.0 - a1) <= angleTolerance) &&
	    (std::abs(90.0 - a2) <= angleTolerance) &&
	    (std::abs(90.0 - a3) <= angleTolerance) &&
	    (std::abs(90.0 - a4) <= angleTolerance));
}

bool
SlapSegIII::Validation::Validate::isRotated(
    const SegmentationPosition &p)
{
	return (
	    (p.tl.x != p.bl.x) ||
	    (p.tr.x != p.br.x) ||
	    (p.tl.y != p.tr.y) ||
	    (p.bl.y != p.br.y));
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
