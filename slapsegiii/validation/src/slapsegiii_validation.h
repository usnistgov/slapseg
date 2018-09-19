/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#ifndef SLAPSEGIII_VALIDATION_H_
#define SLAPSEGIII_VALIDATION_H_

#include <iostream>
#include <random>
#include <string>
#include <type_traits>
#include <vector>

#include <slapsegiii.h>
#include <slapsegiii_validation_data.h>

namespace SlapSegIII
{
	namespace Validation
	{
		/** Operations that this executable can perform. */
		enum class Operation
		{
			/** Perform segmentation */
			Segment,
			/** Print identification information */
			Identify,
			/** Print supported information */
			Supported,
			/** Print usage */
			Usage
		};

		/** Arguments passed on the command line */
		struct Arguments
		{
			/** Number used to seed the random number generator. */
			std::mt19937_64::result_type randomSeed{
			    std::random_device()()};
			/** Operation to be performed. */
			Operation operation{};
			/** Number of processes to run. */
			uint8_t numProcs{1};
		};
		/** Convenience definition for struct Arguments. */
		using Arguments = struct Arguments;

		/**
		 * @brief
		 * Parse command line arguments.
		 *
		 * @param argc
		 * argc from main().
		 * @param argv
		 * argv from main()
		 *
		 * @return
		 * Command line arguments parsed into an Argument.
		 *
		 * @throw std::exception
		 * Errors or inconsistencies when parsing arguments.
		 */
		Arguments
		parseArguments(
		    int argc,
		    char *argv[]);

		/**
		 * @brief
		 * Print the validation driver's usage string to stderr.
		 *
		 * @param name
		 * Name of the executable.
		 */
		void
		printUsage(
		    const std::string &name);

		/**
		 * @brief
		 * Print identification information about a SlapSegIII
		 * implementation.
		 */
		void
		printIdentification();

		/**
		 * @brief
		 * Print what kinds of SlapSegIII images are supported by a
		 * SlapSegIII implementation.
		 */
		void
		printSupported();

		/**
		 * @brief
		 * Test a SlapSegIII's implementation of segmentation for all
		 * supported slap heights.
		 *
		 * @param args
		 * Arguments parsed from command line.
		 */
		void
		testSegmentation(
		    const Arguments &args);

		/**
		 * @brief
		 * Run a set of segmentations.
		 *
		 * @param impl
		 * Pointer to SlapSegIII API implementation.
		 * @param kind
		 * The kind of segmentations in keys.
		 * @param keys
		 * The keys from VALIDATION_DATA to segment.
		 */
		void
		runSegment(
		    std::shared_ptr<Interface> impl,
		    const SlapImage::Kind kind,
		    const std::vector<std::string> &keys);

		/**
		 * @brief
		 * Read a file from disk.
		 *
		 * @param pathName
		 * Path to file to read.
		 *
		 * @return
		 * Contents of pathName as a vector of bytes.
		 *
		 * @throw runtime_error
		 * Error reading from file.
		 */
		std::vector<uint8_t>
		readFile(
		    const std::string &pathName);

		/**
		 * @brief
		 * Sanitize a message for printing in a log file.
		 *
		 * @param message
		 * Message to sanitize.
		 *
		 * @return
		 * Sanitized version of message that can be placed into a
		 * log file.
		 */
		std::string
		sanitizeMessage(
		    const std::string &message);

		/**
		 * @brief
		 * Segment a single image.
		 *
		 * @param impl
		 * Pointer to SlapSegIII implementation.
		 * @param imageName
		 * Name of the image to segment.
		 * @param md
		 * Metadata regarding the image.
		 * @param kind
		 * Kind of image captured.
		 *
		 * @return
		 * Entry for log file.
		 *
		 * @throw
		 * Error reading image or error segmenting.
		 */
		std::string
		segment(
		    const std::shared_ptr<Interface> impl,
		    const std::string &imageName,
		    const ImageMetadata &md,
		    const SlapImage::Kind kind);

		/**
		 * @brief
		 * Create multiple smaller sets from a large set.
		 *
		 * @param combinedSet
		 * Set of strings to split.
		 * @param numSets
		 * Number of sets to create.
		 *
		 * @return
		 * Collection of numSets collections, created sequentially
		 * from combinedSet.
		 *
		 * @throw
		 * numSets > combinedSet.size()
		 */
		std::vector<std::vector<std::string>>
		splitSet(
		    const std::vector<std::string> &combinedSet,
		    uint8_t numSets);

		/**
		 * @brief
		 * Wait for forked children to exit.
		 *
		 * @param numChildren
		 * The expected number of children to exit.
		 */
		void
		waitForExit(
		    const uint8_t numChildren);
	}
}

#endif /* SLAPSEGIII_VALIDATION_H_ */
