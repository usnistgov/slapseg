/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties.  Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain.  NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#ifndef SLAPSEGIII_VALIDATION_VALIDATE_H_
#define SLAPSEGIII_VALIDATION_VALIDATE_H_

#include <bitset>

#include <slapsegiii.h>

namespace SlapSegIII
{
	namespace Validation
	{
		namespace Validate
		{
			/**
			 * Things that could be go wrong within a
			 * SegmentationPosition.
			 */
			enum class ErrorCode : uint8_t
			{
				/** Coordinates form concave shape. */
				IrregularCoordinates,
				/** Coordinates do not form a rectangle. */
				NonRectangularCoordinates,
				/** Rectangle is rotated */
				Rotated,
				/** Coordinates are improperly outside image. */
				CoordinatesOutsideImage,

				/* Number of elements in this enumeration */
				ErrorCode_MAX
			};

			/** Collection of validation errors. */
			using Errors = std::bitset<static_cast<
			    std::underlying_type<ErrorCode>::type>(
			    ErrorCode::ErrorCode_MAX)>;

			/** Things that could be go wrong within a SlapImage/ */
			enum class DeficiencyCode : uint8_t
			{
				/** Coordinates form concave shape. */
				IrregularCoordinates,
				/** Coordinates do not form a rectangle. */
				NonRectangularCoordinates,
				/** Rectangle is rotated */
				Rotated,
				/** Coordinates are improperly outside image. */
				CoordinatesOutsideImage,

				/* Number of elements in this enumeration */
				ErrorCode_MAX
			};

			/** Collection of SlapImage deficiencies. */
			using Deficiencies = std::bitset<static_cast<
			    std::underlying_type<SlapImage::Deficiency>::type>(
			    static_cast<std::underlying_type<
			    SlapImage::Deficiency>::type>(
			    SlapImage::Deficiency::Incomplete) + 1)>;

			/**
			 * @brief
			 * Gather SlapImage deficiencies into a bitset for
			 * printing to the log file.
			 *
			 * @param rs
			 * ReturnStatus returned from segment().
			 *
			 * @return
			 * SlapImage deficiencies as a bitset.
			 */
			Deficiencies
			gatherDeficiencies(
			    const ReturnStatus &rs);

			/**
			 * @brief
			 * Validate SegmentationPosition coordinates against API
			 * rules.
			 *
			 * @param position
			 * The SegmentationPosition to validate.
			 * @param slapImage
			 * SlapImage from which position was generated.
			 *
			 * @return
			 * Detected errors.
			 */
			Errors
			validateSegmentationPosition(
			    const SegmentationPosition &position,
			    std::shared_ptr<SlapImage> slapImage);

			/**
			 * @brief
			 * Determine if the SlapSegIII API allows a
			 * SegmentationPosition to be rotated (theta != 0).
			 *
			 * @param p
			 * SegmentationPosition in question.
			 * @param kind
			 * Kind of image from which positions was derived.
			 *
			 * @return
			 * Whether or not p is permitted to be rotated.
			 */
			bool
			canBeRotated(
			    const SegmentationPosition &p,
			    const SlapImage::Kind kind);

			/**
			 * @brief
			 * Determine if a collection of SegmentationPositions
			 * returned from SlapSegIII::Interface::segment() has
			 * the number of SegmentationPositions required by the
			 * SlapSegIII API.
			 *
			 * @param positions
			 * Collection of SegmentationPositions returned by
			 * SlapSegIII::Interface::segment().
			 * @param kind
			 * Kind of image from which positions was derived.
			 * @param orientation
			 * Orientation of image from which positions was
			 * derived.
			 *
			 * @return
			 * Whether or not positions has the appropriate number
			 * of elements.
			 *
			 * @see getCorrectQuantity()
			 */
			bool
			hasCorrectQuantity(
			    const std::vector<SegmentationPosition> &positions,
			    const SlapImage::Kind kind,
			    const SlapImage::Orientation orientation);

			/**
			 * @brief
			 * Determine the correct number of elements in a
			 * collection of SegmentationPositions returned from
			 * SlapSegIII::Interface::segment(), as required by the
			 * SlapSegIII API.
			 *
			 * @param kind
			 * Kind of image in question.
			 * @param orientation
			 * Orientation of image in question.
			 *
			 * @return
			 * The correct number of elements.
			 */
			uint8_t
			getCorrectQuantity(
			    const SlapImage::Kind kind,
			    const SlapImage::Orientation orientation);

			/**
			 * @brief
			 * Obtain the set of FrictionRidgeGeneralizedPositions
			 * that would be returned in a perfect segmentation.
			 *
			 * @param kind
			 * Kind of image in question.
			 * @param orientation
			 * Orientation of image in question.
			 *
			 * @return
			 * Set of FrictionRidgeGeneralizedPositions that are
			 * expected to be returned if segmenting the type of
			 * image described via kind and orientation.
			 */
			std::set<FrictionRidgeGeneralizedPosition>
			getExpectedFrictionRidgeGeneralizedPositions(
			    const SlapImage::Kind kind,
			    const SlapImage::Orientation orientation);

			/**
			 * @brief
			 * Determine if a SegmentationPosition has coordinates
			 * outside of the image.
			 *
			 * @param p
			 * SegmentationPosition in question.
			 * @param width
			 * Width of the image from which p was derived.
			 * @param height
			 * Height of the image from which p was derived.
			 *
			 * @return
			 * Whether or not p has coordinates outside the image.
			 */
			bool
			isOutsideImage(
			    const SegmentationPosition &p,
			    const uint16_t width,
			    const uint16_t height);

			/**
			 * @brief
			 * Determine if the shape created by a
			 * SegmentationPosition is rectangular.
			 *
			 * @param p
			 * SegmentationPosition in question.
			 *
			 * @return
			 * Whether or not the shape formed by p is rectangular.
			 */
			bool
			isRectangular(
			    const SegmentationPosition &p);

			/**
			 * @brief
			 * Determine if the shape created by a
			 * SegmentationPosition is rotated.
			 *
			 * @param p
			 * SegmentationPosition in question.
			 *
			 * @return
			 * Whether or not the shape formed p is rotated.
			 *
			 * @note
			 * Assumes the shape is rectangular.
			 */
			bool
			isRotated(
			    const SegmentationPosition &p);

			/**
			 * @brief
			 * Whether or not the coordinates in a
			 * SegmentationPosition respect their name.
			 * @details
			 * "Top" coordinates are above "bottom" coordinates
			 * and "left" coordinates are left of "right"
			 * coordinates.
			 *
			 * @param p
			 * SegmentationPosition in question.
			 *
			 * @return
			 * Whether or not the coordinates in p respect their
			 * name.
			 */
			bool
			hasIrregularCoordinates(
			    const SegmentationPosition &p);
		}
	}
}

#endif /* SLAPSEGIII_VALIDATION_VALIDATE_H_ */
