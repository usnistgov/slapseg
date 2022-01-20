/**
 * @mainpage
 * @section Overview
 * This is the API that must be implemented to participate in the National
 * Institute of Standards and Technology (NIST)'s [Slap Fingerprint Segmentation
 * III Evaluation](https://www.nist.gov/itl/iad/image-group/slapsegiii)
 * (%SlapSegIII).
 *
 * @section Implementation
 * A pure-virtual (abstract) class called SlapSegIII::Interface has been
 * created. Participants must implement all methods of SlapSegIII::Interface in
 * a subclass, and submit this implementation as a shared library. The name of
 * the library must follow the instructions outlined in the test plan, and be
 * identical to the information returned from
 * SlapSegIII::Interface::getIdentification(). A test application will link
 * against the submitted library, instantiate an instance of the implementation
 * by calling SlapSegIII::Interface::getImplementation(), and perform various
 * segmentation operations.
 *
 * @section Contact
 * Additional information regarding the Slap Fingerprint Segmentation Evaluation
 * by emailing the test liaisons at slapseg@nist.gov.
 *
 * @section License
 * This software was developed at NIST by employees of the Federal Government
 * in the course of their official duties. Pursuant to title 17 Section 105 of
 * the United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#ifndef SLAPSEGIII_H_
#define SLAPSEGIII_H_

#include <filesystem>
#include <memory>
#include <set>
#include <string>
#include <vector>

/** Slap Fingerprint Segmentation Evaluation III namespace. */
namespace SlapSegIII
{
	/** Friction ridge generalized position. */
	enum class FrictionRidgeGeneralizedPosition
	{
		/** Unknown. */
		Unknown = 0,
		/** Right thumb. */
		RightThumb = 1,
		/** Right index. */
		RightIndex = 2,
		/** Right middle. */
		RightMiddle = 3,
		/** Right ring. */
		RightRing = 4,
		/** Right little. */
		RightLittle = 5,
		/** Left thumb. */
		LeftThumb = 6,
		/** Left index. */
		LeftIndex = 7,
		/** Left middle. */
		LeftMiddle = 8,
		/** Left ring. */
		LeftRing = 9,
		/** Left little. */
		LeftLittle = 10
	};

	/** A 2D coordinate, assuming an origin at the top left. */
	struct Coordinate
	{
		/**
		 * Coordinate constructor.
		 *
		 * @param x
		 * X coordinate.
		 * @param y
		 * Y coordinate.
		 */
		Coordinate(
		    const int32_t x = 0,
		    const int32_t y = 0)
		    noexcept;

		/** X coordinate */
		int32_t x{};
		/** Y coordinate */
		int32_t y{};
	};

	/** Data and metadata of an image containing multiple fingerprints. */
	struct SlapImage
	{
		/** Kind of slap image */
		enum class Kind
		{
			/** Tenprint card (FRGP 13, 14) */
			TwoInch = 2,
			/** Identification Flats (FRGP 13, 14, 15) */
			ThreeInch = 3,
			/** Upper palm (FRGP 15, 26, 28) */
			UpperPalm = 5,
			/** Full palm (FRGP 21, 23) */
			FullPalm = 8
		};

		/** Friction ridge capture technology. */
		enum class CaptureTechnology
		{
			/** Unknown. */
			Unknown = 0,
			/** Scanned ink on paper. */
			ScannedInkOnPaper = 2,
			/** Optical, total internal reflection, bright field. */
			OpticalTIRBright = 3
		};

		/** Hand orientation being segmented. */
		enum class Orientation
		{
			/** Right hand. */
			Right = 0,
			/** Left hand. */
			Left = 1,
			/** Two thumbs. */
			Thumbs = 2
		};

		/** Reasons that a SlapImage cannot be reliably segmented. */
		enum class Deficiency
		{
			/**
			 * Moisture/condensation "halos," "ghost"
			 * impressions, etc.
			 */
			Artifacts = 0,
			/** Low contrast, over-inked, too much pressure, etc. */
			ImageQuality,
			/**
			 * Hand placed sideways, upside-down, or is
			 * overly-rotated in impressions where rotation is not
			 * expected.
			 */
			HandGeometry,
			/**
			 * Overwhelming majority of expected ridge structure
			 * is not present.
			 */
			Incomplete
		};

		/** Default SlapImage constructor. */
		SlapImage();

		/**
		 * @brief
		 * SlapImage constructor.
		 *
		 * @param width
		 * Width of the image.
		 * @param height
		 * Height of the image.
		 * @param ppi
		 * Resolution of the image in pixels per inch.
		 * @param kind
		 * The kind of capture employed to create the image.
		 * @param captureTechnology
		 * The capture technology used to create the image.
		 * @param orientation
		 * The orientation of fingers in the image.
		 * @param pixels
		 * `width` * `height` bytes of image data, with `pixels.front()`
		 * representing the top-left pixel and `pixels.back()`
		 * representing the bottom-right pixel. It is raw 8 bits/pixel
		 * grayscale image data, canonically coded as defined in ISO/IEC
		 * 19794-4:2005, section 6.2.
		 */
		SlapImage(
		    const uint16_t width,
		    const uint16_t height,
		    const uint16_t ppi,
		    const Kind kind,
		    const CaptureTechnology captureTechnology,
		    const Orientation orientation,
		    const std::vector<uint8_t> &pixels);

		/** Width of the image. */
		uint16_t width{};
		/** Height of the image. */
		uint16_t height{};
		/** Resolution of the image in pixels per inch. */
		uint16_t ppi{};
		/** The kind of capture employed to create the image. */
		Kind kind{};
		/**  The capture technology used to create the image. */
		CaptureTechnology captureTechnology{};
		/** The orientation of fingers in the image. */
		Orientation orientation{};
		/**
		 * `width` * `height` bytes of image data, with `pixels.front()`
		 * representing the top-left pixel and `pixels.back()`
		 * representing the bottom-right pixel. It is raw 8 bits/pixel
		 * grayscale image data, canonically coded as defined in ISO/IEC
		 * 19794-4:2005, section 6.2.
		 *
		 * @note
		 * To pass pixels to a C-style array, invoke pixel's `data()`
		 * method (`pixels.data()`).
		 */
		std::vector<uint8_t> pixels{};
	};

	/** Representation of a segmentation position. */
	struct SegmentationPosition
	{
		/** An individual segmentation position discovery. */
		struct Result
		{
			/** Possible results of segmenting a single finger. */
			enum class Code
			{
				/** Success. */
				Success = 0,
				/** Finger not found to segment. */
				FingerNotFound,
				/** Finger present, but can't be segmented. */
				FailedToSegment,
				/** Failure: Other reason. See error message. */
				VendorDefined
			};

			/**
			 * Result constructor.
			 *
			 * @param code
			 * Status from segmenting an individual finger.
			 * @param message
			 * Message providing insight into code's value.
			 */
			Result(
			    const Code code = Code::Success,
			    const std::string &message = "");

			/** Status from segmenting an individual finger. */
			Code code{};
			/** Message providing insight into code's value. */
			std::string message{};
		};

		/** Default SegmentationPosition constructor. */
		SegmentationPosition();

		/**
		 * @brief
		 * SegmentationPosition constructor.
		 *
		 * @param frgp
		 * FrictionRidgeGeneralizedPosition for the bounded finger.
		 * @param tl
		 * Top-left coordinate, where *top* refers to the top of the
		 * fingerprint.
		 * @param tr
		 * Top-right coordinate, where *top* refers to the top of the
		 * fingerprint.
		 * @param bl
		 * Bottom-left coordinate, where *bottom* refers to distal
		 * interphalangeal joint of the fingerprint.
		 * @param br
		 * Bottom-right coordinate, where *bottom* refers to distal
		 * interphalangeal joint of the fingerprint.
		 * @param result
		 * Segmentation result.
		 */
		SegmentationPosition(
		    const FrictionRidgeGeneralizedPosition frgp,
		    const Coordinate &tl,
		    const Coordinate &tr,
		    const Coordinate &bl,
		    const Coordinate &br,
		    const Result result = {});

		/** Friction ridge generalized position */
		FrictionRidgeGeneralizedPosition frgp{};
		/**
		 * Top-left coordinate, where *top* refers to the top of the
		 * fingerprint.
		 */
		Coordinate tl{};
		/**
		 * Top-right coordinate, where *top* refers to the top of the
		 * fingerprint.
		 */
		Coordinate tr{};
		/**
		 * Bottom-left coordinate, where *bottom* refers to distal
		 * interphalangeal joint of the fingerprint.
		 */
		Coordinate bl{};
		/**
		 * Bottom-right coordinate, where *bottom* refers to distal
		 * interphalangeal joint of the fingerprint.
		 */
		Coordinate br{};
		/** Result of segmentation position discovery. */
		Result result{};
	};

	/** Information about the execution of an API method. */
	struct ReturnStatus
	{
		/** Possible exit status of a SlapSegIII API method. */
		enum class Code
		{
			/** Success. */
			Success = 0,
			/** Failure: Image data was not parsable. */
			InvalidImageData,
			/**
			  * Failure: Image cannot be reliably segmented and
			  * should be recaptured. No best-effort segmentation
			  * attempt will be provided. At least one Deficiency
			  * shall be specified.
			  */
			RequestRecapture,
			/**
			  * Failure: Image cannot be reliably segmented and
			  * should be recaptured. Best-effort segmentation was
			  * attempted. At least one Deficiency shall be
			  * specified.
			  */
			RequestRecaptureWithAttempt,
			/** Failure: Image resolution not supported. */
			UnsupportedResolution,
			/** Failure: Slap type is not supported. */
			UnsupportedSlapType,
			/** Failure: This method is not implemented. */
			NotImplemented,
			/** Failure: Other reason. See error message. */
			VendorDefined
		};

		/**
		 * @brief
		 * ReturnStatus constructor.
		 *
		 * @param code
		 * Status code from a method.
		 * @param imageDeficiencies
		 * If code is RequestRecapture or RequestRecaptureWithAttempt,
		 * one or more Deficiency describing why a recapture should
		 * be requested.
		 * @param message
		 * Message providing insight into code's value.
		 *
		 * @note
		 * If successful, returning from a method can be accomplished
		 * by `return {};`.
		 */
		ReturnStatus(
		    const Code code = Code::Success,
		    const std::set<SlapImage::Deficiency>
		    &imageDeficiencies = {},
		    const std::string &message = "");

		/** Returned status code. */
		Code code{};
		/**
		 * Deficiencies with a SlapImage (required only when code is
		 * RequestRecapture or RequestRecaptureWithAttempt).
		 */
		std::set<SlapImage::Deficiency> imageDeficiencies{};
		/** Explanatory message (optional). */
		std::string message{};
	};

	/** Information identifying this submission. */
	struct SubmissionIdentification
	{
		/** Default constructor. */
		SubmissionIdentification();

		/**
		 * @brief
		 * SubmissionIdentification constructor.
		 *
		 * @param libraryIdentifier
		 * Identifier for this submission.
		 * @param version
		 * Unique version number for this submission.
		 * @param marketingIdentifier
		 * Optional marketing name for this submission.
		 */
		SubmissionIdentification(
		    const std::string &libraryIdentifier,
		    const uint16_t version,
		    const std::string &marketingIdentifier = "");

		/**
		 * @brief
		 * Version number for this submission.
		 *
		 * @details
		 * Must be unique with every new submission. Must match the
		 * final underscore-delimited token of the filename of the
		 * core library.
		 */
		uint16_t version{};
		/**
		 * @brief
		 * Identifier for this submission.
		 *
		 * @details
		 * Should be the same for all submissions from the same
		 * organization. Case-sensitive. Must match the regular
		 * expression "[:alnum:]+". Must be the same as the second
		 * underscore-delimited token of the filename of the core
		 * library.
		 */
		std::string libraryIdentifier{};
		/**
		 * @brief
		 * Marketing name for this submission.
		 *
		 * @details
		 * Optional. Must match the regular expression "[[:graph:] ]*".
		 */
		std::string marketingIdentifier{};

	};

	/**
	 * @brief
	 * Slap Fingerprint Segmentation III interface.
	 *
	 * @details
	 * Participants must inherit from this class and implement all methods.
	 */
	class Interface
	{
	public:
		/**
		 * @brief
		 * Obtain identification and version information for this
		 * submission.
		 *
		 * @return
		 * SubmissionIdentification with identification details about
		 * this submission.
		 *
		 * @note
		 * This method shall return instantly.
		 */
		virtual
		SubmissionIdentification
		getIdentification()
		    const = 0;

		/**
		 * @brief
		 * Obtain features supported by this implementation.
		 *
		 * @return
		 * A tuple whose first member is the set of supported
		 * SlapImage::Kind and whose second member indicates the
		 * presence of an implementation of
		 * Interface::determineOrientation().
		 *
		 * @note
		 * Returned set of SlapImage::Kind must include at least one
		 * SlapImage::Kind.
		 *
		 * @note
		 * This method shall return instantly.
		 * @note
		 * If an orientation determination algorithm is not present
		 * (e.g., the second member of the return value from this
		 * method is false), the implementation must still provide an
		 * implementation for Interface::determineOrientation() that
		 * returns ReturnStatus::Code::NotImplemented.
		 */
		virtual
		std::tuple<std::set<SlapImage::Kind>, bool>
		getSupported()
		    const = 0;

		/**
		 * @brief
		 * Discover fingerprint segmentation positions within a slap
		 * image.
		 *
		 * @param image
		 * Image data and metadata to segment.
		 *
		 * @return
		 * A tuple whose first member is ReturnStatus (with
		 * ReturnStatus.code set to ReturnStatus::Code::Success when
		 * successful, or another ReturnStatus::Code and reason
		 * ReturnStatus.message on failure) and whose second member is
		 * a std::vector containing a SegmentationPosition for each
		 * segmentation position found within image.
		 *
		 * @note
		 * In failure situations, a best-effort segmentation shall
		 * still be performed. Some images may be provided whose
		 * content should cause an implementation to reject the image.
		 * This is in support of an evaluation goal to determine if an
		 * implementation can correctly determine when to issue a
		 * request for recapture and specify the reasoning. If the
		 * implementation incorrectly determines a failure, the
		 * best-effort segmentation positions can still be scored.
		 *
		 * @note
		 * This method shall return on average within the time limits
		 * specified in Table 8 of the %SlapSegIII Test Plan.
		 *
		 * @note
		 * Be cognizant of semantics in returned SegmentationPosition.
		 * Fingers in TwoInch should be rotated identically while
		 * UpperPalm and FullPalm may be rotated independently.
		 * ThreeInch shall not be rotated. See Section 3 of the
		 * %SlapSegIII Test Plan for more details.
		 */
		virtual
		std::tuple<ReturnStatus, std::vector<SegmentationPosition>>
		segment(
		    const SlapImage &image) = 0;

		/**
		 * @brief
		 * Determine the hand orientation of a slap image.
		 *
		 * @param image
		 * Image data and metadata.
		 *
		 * @return
		 * A tuple whose first member is ReturnStatus (with
		 * ReturnStatus.code set to ReturnStatus::Code::Success when
		 * successful, or another ReturnStatus::Code and reason
		 * ReturnStatus.message on failure) and whose second member is
		 * the hypothesized SlapImage::Orientation of `image`.
		 *
		 * @warning
		 * Do not consult the `orientation` member of `image`. It will
		 * be default-initialized only.
		 *
		 * @note
		 * Although this method must be implemented,
		 * ReturnStatus::Code::NotImplemented can be set if no
		 * orientation determination algorithm is available. A possible
		 * implementation for participants who will not support this
		 * method might be:
		 * `return std::make_tuple(ReturnStatus::Code::NotImplemented,
		 *      SlapImage::Orientation{});`
		 */
		virtual
		std::tuple<ReturnStatus, SlapImage::Orientation>
		determineOrientation(
		    const SlapImage &image) = 0;

		/** Destructor. */
		virtual ~Interface();

		/**
		 * @brief
		 * Obtain a managed pointer to an object implementing
		 * SlapSegIII::Interface.
		 *
		 * @param configurationDirectory
		 * Path to a read-only directory populated with configuration
		 * files provided in validation.
		 *
		 * @return
		 * Shared pointer to an instance of Interface containing the
		 * participant's segmentation algorithm.
		 *
		 * @note
		 * A possible implementation might be:
		 * `return (std::make_shared<Implementation>(
		 *  configurationDirectory));`
		 * @note
		 * `configurationDirectory` may be stored on a slow disk. NIST
		 * suggests reading data into available RAM.
		 *
		 * @note
		 * This method shall return in <= 10 seconds.
		 */
		static
		std::shared_ptr<Interface>
		getImplementation(
		    const std::filesystem::path &configurationDirectory);
	};

	/*
	 * API versioning.
	 *
	 * NIST code will extern the version number symbols. Participant code
	 * shall compile them into their core library.
	 */
	#ifdef NIST_EXTERN_API_VERSION
	/** API major version number. */
	extern uint16_t API_MAJOR_VERSION;
	/** API minor version number. */
	extern uint16_t API_MINOR_VERSION;
	/** API patch version number. */
	extern uint16_t API_PATCH_VERSION;
	#else /* NIST_EXTERN_API_VERSION */
	/** API major version number. */
	uint16_t API_MAJOR_VERSION{1};
	/** API minor version number. */
	uint16_t API_MINOR_VERSION{2};
	/** API patch version number. */
	uint16_t API_PATCH_VERSION{0};
	#endif /* NIST_EXTERN_API_VERSION */
}

#endif /* SLAPSEGIII_H_ */
