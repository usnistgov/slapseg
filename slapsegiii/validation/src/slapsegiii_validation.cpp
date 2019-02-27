/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#include <sys/stat.h>
#include <sys/wait.h>

#include <getopt.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <system_error>
#include <thread>

#include <slapsegiii_validation.h>
#include <slapsegiii_validation_data.h>
#include <slapsegiii_validation_validate.h>
#include <slapsegiii_validation_utils.h>

std::string
SlapSegIII::Validation::determineOrientation(
    const std::shared_ptr<Interface> impl,
    const std::string &imageName,
    const Validation::ImageMetadata &md,
    const SlapImage::Kind kind)
{
	std::shared_ptr<SlapImage> si;
	try {
		si = std::make_shared<SlapImage>(md.width, md.height, md.ppi,
		    kind, md.captureTechnology, SlapImage::Orientation{},
		    readFile(IMAGE_DIR + '/' + imageName));
	} catch (const std::exception &e) {
		throw std::runtime_error("Error reading " + imageName + " (" +
		    e.what() + ")");
	}

	std::tuple<ReturnStatus, SlapImage::Orientation> rv{};
	std::chrono::steady_clock::time_point start{}, stop{};
	try {
		start = std::chrono::steady_clock::now();
		rv = impl->determineOrientation(*si.get());
		stop = std::chrono::steady_clock::now();
	} catch (const std::exception &e) {
		throw std::runtime_error("Exception while determining "
		    "orientation of " + imageName + " (" + e.what() + ")");
	} catch (...) {
		throw std::runtime_error("Exception while determining "
		    "orientation of " + imageName);
	}

	const auto elapsed = std::to_string(std::chrono::duration_cast<
	    std::chrono::microseconds>(stop - start).count());

	std::string logLine{imageName + ',' + elapsed + ',' +
	    e2i2s(std::get<0>(rv).code) + ',' +
	    sanitizeMessage(std::get<0>(rv).message) + ','};
	if (std::get<0>(rv).code == ReturnStatus::Code::Success)
		logLine += e2i2s(std::get<1>(rv)) + '\n';
	else
		logLine += "NA\n";

	return (logLine);
}

void
SlapSegIII::Validation::printUsage(
    const std::string &name)
{
	std::cerr << "Usage:\t" << name << " -i(dentify)\n";
	std::cerr << "\t" << name << " -k(inds of images supported)\n";
	std::cerr << "\t" << name << " -s(egment) [-r random_seed] "
	    "[-f num_procs]\n";

	const std::string blankName(name.size(), ' ');
	std::cerr << "\t" << name << " -d(etermine orientation) "
	    "[-r random_seed]\n\t" + blankName + " [-f num_procs]\n";
}

SlapSegIII::Validation::Arguments
SlapSegIII::Validation::parseArguments(
    int argc,
    char *argv[])
{
	static const char options[] {"ikr:sf:d"};

	bool seenOperation{false};
	Validation::Arguments args{};

	int c{};
	while ((c = getopt(argc, argv, options)) != -1) {
		switch (c) {
		case 'i':	/* Print information */
			if (seenOperation)
				throw std::logic_error{"Multiple operations "
				    "specified"};
			seenOperation = true;

			args.operation = Operation::Identify;
			break;
		case 'k':	/* Print supported kinds */
			if (seenOperation)
				throw std::logic_error{"Multiple operations "
				    "specified"};
			seenOperation = true;

			args.operation = Operation::Supported;
			break;
		case 's':	/* Perform segmentation */
			if (seenOperation)
				throw std::logic_error{"Multiple operations "
				    "specified"};
			seenOperation = true;

			args.operation = Operation::Segment;
			break;
		case 'r':	/* Random seed */
			try {
				args.randomSeed = std::stoll(optarg);
			} catch (std::exception) {
				throw std::invalid_argument{"Random seed (-r): "
				    "an error occurred when parsing \"" +
				    std::string(optarg) + "\""};
			}
			break;
		case 'f': {	/* Number of processes */
			try {
				args.numProcs = std::stol(optarg);
			} catch (std::exception) {
				throw std::invalid_argument{"Number of "
				    "processes (-f): an error occurred when "
				    "parsing \"" + std::string(optarg) + "\""};
			}

			const auto threadCount = std::thread::
			    hardware_concurrency();
			if ((threadCount == 0 && args.numProcs > 4) ||
			    (args.numProcs > threadCount))
				throw std::invalid_argument{"Number of "
				    "processes (-f): Asked to spawn " +
				    std::to_string(args.numProcs) + " "
				    "processes, but refusing"};
			break;
		}
		case 'd':
			if (seenOperation)
				throw std::logic_error{"Multiple operations "
				    "specified"};
			seenOperation = true;

			args.operation = Operation::Orientation;
			break;
		}
	}

	if (!seenOperation)
		args.operation = Operation::Usage;

	return (args);
}

void
SlapSegIII::Validation::printIdentification()
{
	const auto id = SlapSegIII::Interface::getImplementation()->
	    getIdentification();

	std::cout << "LibraryIdentifier = " << id.libraryIdentifier << '\n' <<
	    "MarketingIdentifier = " << id.marketingIdentifier << '\n' <<
	    "Version = 0x" << std::setw(4) << std::setfill('0') << std::hex <<
	    id.version << '\n';
}

void
SlapSegIII::Validation::printSupported()
{
	const auto rv = SlapSegIII::Interface::getImplementation()->
	    getSupported();
	const auto kinds = std::get<0>(rv);

	std::cout << std::boolalpha <<
	    "TwoInch = " <<
	    (kinds.find(SlapImage::Kind::TwoInch) != kinds.cend()) <<
	    "\nThreeInch = " <<
	    (kinds.find(SlapImage::Kind::ThreeInch) != kinds.cend()) <<
	    "\nUpperPalm = " <<
	    (kinds.find(SlapImage::Kind::UpperPalm) != kinds.cend()) <<
	    "\nFullPalm = " <<
	    (kinds.find(SlapImage::Kind::FullPalm) != kinds.cend()) <<
	    "\nDetermineOrientation = " << std::get<1>(rv) << '\n';
}

std::vector<uint8_t>
SlapSegIII::Validation::readFile(
    const std::string &pathName)
{
	std::ifstream file{pathName,
	    std::ifstream::ate | std::ifstream::binary};
	if (!file)
		throw std::runtime_error{"Could not open " + pathName};
	file.unsetf(std::ifstream::skipws);

	const auto size = file.tellg();
	if (size == -1)
		throw std::runtime_error{"Could not open " + pathName};

	std::vector<uint8_t> buf{};
	buf.reserve(size);

	file.seekg(std::ifstream::beg);
	buf.insert(buf.begin(), std::istream_iterator<uint8_t>(file),
	    std::istream_iterator<uint8_t>());

	return (buf);
}

void
SlapSegIII::Validation::runSegment(
    std::shared_ptr<Interface> impl,
    const SlapImage::Kind kind,
    const std::vector<std::string> &keys)
{
	std::ofstream file("output/segments-" + e2i2s(kind) + '-' +
	    std::to_string(getpid()) + ".log");
	if (!file) {
		throw std::runtime_error(std::to_string(getpid()) + ": Error "
		    "creating log file");
	}

	static const std::string header{"name,elapsed,rCode,\"rMessage\",frgp,"
	    "tlx,tly,trx,try,blx,bly,brx,bry,sCode,\"sMessage\",\"errors\","
	    "\"deficiencies\",correctQuantity"};
	file << header << '\n';
	if (!file)
		throw std::runtime_error(std::to_string(getpid()) + ": Error "
		    "writing to log");

	for (const auto &imageName : keys) {
		const auto md = VALIDATION_DATA.at(kind).at(imageName);
		file << segment(impl, imageName, md, kind);

		if (!file)
			throw std::runtime_error(std::to_string(getpid()) +
			    ": Error writing to log");
	}
}

void
SlapSegIII::Validation::runDetermineOrientation(
    std::shared_ptr<Interface> impl,
    const SlapImage::Kind kind,
    const std::vector<std::string> &keys)
{
	/* Don't run if implementation does not claim support */
	if (!std::get<1>(impl->getSupported()))
		return;

	std::ofstream file("output/orientation-" + e2i2s(kind) + '-' +
	    std::to_string(getpid()) + ".log");
	if (!file) {
		throw std::runtime_error(std::to_string(getpid()) + ": Error "
		    "creating log file");
	}

	static const std::string header{"name,elapsed,rCode,\"rMessage\","
	    "orientation"};
	file << header << '\n';
	if (!file)
		throw std::runtime_error(std::to_string(getpid()) + ": Error "
		    "writing to log");

	for (const auto &imageName : keys) {
		const auto md = VALIDATION_DATA.at(kind).at(imageName);
		file << determineOrientation(impl, imageName, md, kind);

		if (!file)
			throw std::runtime_error(std::to_string(getpid()) +
			    ": Error writing to log");
	}
}

std::string
SlapSegIII::Validation::sanitizeMessage(
    const std::string &message)
{
	if (message.empty())
		return {"\"\""};

	std::string sanitized{message};

	/* Replace characters not matching the documented regex with space */
	auto it = sanitized.begin();
	while ((it = std::find_if_not(sanitized.begin(), sanitized.end(),
	    [](const char &c) -> bool {
		return (std::isgraph(c) || c == ' ');
	    })) != sanitized.end()) {
		sanitized.replace(it, std::next(it), " ");
	}

	/* Replace " with \" (we log to quoted CSV columns) */
	static const std::string from{"\""};
	static const std::string to{"\\\""};
	std::string::size_type position{0};
	while ((position = sanitized.find(from, position)) !=
	    std::string::npos) {
		sanitized.replace(position, from.length(), to);
		position += to.length();
	}
	return ('"' + sanitized + '"');
}

std::string
SlapSegIII::Validation::segment(
    const std::shared_ptr<Interface> impl,
    const std::string &imageName,
    const Validation::ImageMetadata &md,
    const SlapImage::Kind kind)
{
	std::shared_ptr<SlapImage> si;
	try {
		si = std::make_shared<SlapImage>(md.width, md.height, md.ppi,
		    kind, md.captureTechnology, md.orientation,
		    readFile(IMAGE_DIR + '/' + imageName));
	} catch (const std::exception &e) {
		throw std::runtime_error("Error reading " + imageName + " (" +
		    e.what() + ")");
	}

	std::tuple<ReturnStatus, std::vector<SegmentationPosition>> rv{};
	std::chrono::steady_clock::time_point start{}, stop{};
	try {
		start = std::chrono::steady_clock::now();
		rv = impl->segment(*si.get());
		stop = std::chrono::steady_clock::now();
	} catch (const std::exception &e) {
		throw std::runtime_error("Exception while segmenting " +
		    imageName + " (" + e.what() + ")");
	} catch (...) {
		throw std::runtime_error("Exception while segmenting " +
		    imageName);
	}

	/* Enforce correct reporting of image deficiencies */
	if (((std::get<0>(rv).code == ReturnStatus::Code::RequestRecapture) ||
	    (std::get<0>(rv).code ==
	    ReturnStatus::Code::RequestRecaptureWithAttempt)) &&
	    (std::get<0>(rv).imageDeficiencies.size() == 0))
		throw std::runtime_error("At least one image deficiency must "
		    "be set if requesting recapture for image " + imageName);
	if ((std::get<0>(rv).imageDeficiencies.size() != 0) &&
	    (std::get<0>(rv).code != ReturnStatus::Code::RequestRecapture) &&
	    (std::get<0>(rv).code !=
	    ReturnStatus::Code::RequestRecaptureWithAttempt))
		throw std::runtime_error("Cannot declare an image deficiency "
		    "if not requesting a recapture for image " + imageName);

	const auto elapsed = std::to_string(std::chrono::duration_cast<
	    std::chrono::microseconds>(stop - start).count());

	std::string logLine{};
	if (std::get<0>(rv).code == ReturnStatus::Code::Success ||
	    std::get<0>(rv).code ==
	    ReturnStatus::Code::RequestRecaptureWithAttempt) {
		const auto correctQuantity = std::to_string(
		    Validate::hasCorrectQuantity(std::get<1>(rv),
		    md.orientation));

		for (const auto &pos : std::get<1>(rv))
			logLine += imageName + ',' + elapsed + ',' +
			    e2i2s(std::get<0>(rv).code) + ',' +
			    sanitizeMessage(std::get<0>(rv).message) + ',' +
			    e2i2s(pos.frgp) + ',' + ts(pos.tl.x) + ',' +
			    ts(pos.tl.y) + ',' + ts(pos.tr.x) + ',' +
			    ts(pos.tr.y) + ',' + ts(pos.bl.x) + ',' +
			    ts(pos.bl.y) + ',' + ts(pos.br.x) + ',' +
			    ts(pos.br.y) + ',' + e2i2s(pos.result.code) + ',' +
			    sanitizeMessage(pos.result.message) + ",\"" +
			    Validate::validateSegmentationPosition(pos, si).
			    to_string() + "\",\"" + Validate::
			    gatherDeficiencies(std::get<0>(rv)).to_string() +
			    "\"," + correctQuantity + '\n';
	} else {
		for (const auto &frgp :
		    Validate::getExpectedFrictionRidgeGeneralizedPositions(
		    md.orientation))
			logLine += imageName + ',' + elapsed + ',' +
			    e2i2s(std::get<0>(rv).code) + ',' +
			    sanitizeMessage(std::get<0>(rv).message) + ',' +
			    e2i2s(frgp) + ",NA,NA,NA,NA,NA,NA,NA,NA,NA,\"\","
			    "\"\",\"\",NA\n";
	}

	return (logLine);
}

std::vector<std::vector<std::string>>
SlapSegIII::Validation::splitSet(
    const std::vector<std::string> &combinedSet,
    uint8_t numSets)
{
	if (numSets == 0)
		return {};
	if (numSets == 1)
		return {combinedSet};

	const std::vector<std::string>::size_type size = static_cast<
	    std::vector<std::string>::size_type>(
	    std::ceil(combinedSet.size() / static_cast<float>(numSets)));
	if (size < numSets)
		throw std::invalid_argument("Too many sets.");

	std::vector<std::vector<std::string>> sets{};
	sets.reserve(numSets);
	for (uint8_t i{0}; i < numSets; ++i)
		sets.emplace_back(std::next(combinedSet.begin(), size * i),
		    std::next(combinedSet.begin(), std::min(size * (i + 1),
		    combinedSet.size())));

	return (sets);
}

void
SlapSegIII::Validation::testOperation(
    const Validation::Arguments &args)
{
        auto rng = std::mt19937_64(args.randomSeed);

	const auto impl = SlapSegIII::Interface::getImplementation();
	const auto kinds = std::get<0>(impl->getSupported());
	for (const auto &kind : kinds) {
		/* Shuffle images of each Kind */
		std::vector<std::string> imageNames{};
		imageNames.reserve(Validation::VALIDATION_DATA.at(kind).size());
		for (const auto &i : Validation::VALIDATION_DATA.at(kind))
			imageNames.push_back(i.first);
		std::shuffle(imageNames.begin(), imageNames.end(), rng);

		if (args.numProcs <= 1) {
			switch (args.operation) {
			case Operation::Segment:
				runSegment(impl, kind, imageNames);
				break;
			case Operation::Orientation:
				runDetermineOrientation(impl, kind, imageNames);
				break;
			default:
				throw std::runtime_error("Invalid operation "
				    "sent to testOperation()");
			}
		} else {
			/* Split into multiple sets of images */
			const auto sets = splitSet(imageNames, args.numProcs);

			/* Fork. */
			for (const auto &set : sets) {
				const auto pid = fork();
				switch (pid) {
				case 0:		/* Child */
					try {
						switch (args.operation) {
						case Operation::Segment:
							runSegment(impl, kind,
							    set);
							break;
						case Operation::Orientation:
							runDetermineOrientation(
							    impl, kind, set);
							break;
						default:
							throw std::
							    runtime_error(
							    "Invalid operation "
							    "sent to test"
							    "Operation()");
						}
					} catch (const std::exception &e) {
						std::cerr << e.what() << '\n';
						std::exit(1);
					} catch (...) {
						std::cerr << "Caught unknown "
						    "exception\n";
						std::exit(1);
					}
					std::exit(0);

					/* Not reached */
					break;
				case -1:	/* Error */
					throw std::runtime_error("Error during "
					    "fork()");
				default:	/* Parent */
					break;
				}
			}

			waitForExit(args.numProcs);
		}
	}
}

void
SlapSegIII::Validation::waitForExit(
    const uint8_t numChildren)
{
	pid_t pid{-1};
	bool stop{false};
	uint8_t exitedChildren{0};
	int status{};
	while (exitedChildren != numChildren) {
		stop = false;
		while (!stop) {
			pid = ::wait(&status);
			switch (pid) {
			case 0:		/* Status not available */
				break;
			case -1:	/* Delivery of signal */
				switch (errno) {
				case ECHILD:	/* No child processes remain */
					stop = true;
					break;
				case EINTR:	/* Interruption, try again */
					break;
				default:
					throw std::runtime_error{"Error while "
					    "reaping: " + std::system_error(
					        errno, std::system_category()).
					        code().message()};
				}
				break;
			default:	/* Child exited */
				++exitedChildren;
				break;
			}
		}
	}
}

int
main(
    int argc,
    char *argv[])
{
	int rv = EXIT_FAILURE;

	if ((SlapSegIII::API_MAJOR_VERSION != 1) ||
	    (SlapSegIII::API_MINOR_VERSION != 0)) {
		std::cerr << "Incompatible API version encountered.\n "
		    "- Validation: 1.0.*\n - Participant: " <<
		    SlapSegIII::API_MAJOR_VERSION << '.' <<
		    SlapSegIII::API_MINOR_VERSION << '.' <<
		    SlapSegIII::API_PATCH_VERSION << '\n';
		return (rv);
	}

	SlapSegIII::Validation::Arguments args{};
	try {
		args = SlapSegIII::Validation::parseArguments(argc, argv);
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
		SlapSegIII::Validation::printUsage(argv[0]);
		return (rv);
	}

	switch (args.operation) {
	case SlapSegIII::Validation::Operation::Identify:
		try {
			SlapSegIII::Validation::printIdentification();
			rv = EXIT_SUCCESS;
		} catch (const std::exception &e) {
			std::cerr << "Interface::getIdentification(): " <<
			    e.what() << '\n';
		} catch (...) {
			std::cerr << "Interface::getIdentification(): "
			    "Non-standard exception\n";
		}
		break;
	case SlapSegIII::Validation::Operation::Segment:
		try {
			SlapSegIII::Validation::testOperation(args);
			rv = EXIT_SUCCESS;
		} catch (const std::exception &e) {
			std::cerr << "Interface::segment(): " <<
			    e.what() << '\n';
		} catch (...) {
			std::cerr << "Interface::segment(): "
			    "Non-standard exception\n";
		}
		break;
	case SlapSegIII::Validation::Operation::Supported:
		try {
			SlapSegIII::Validation::printSupported();
			rv = EXIT_SUCCESS;
		} catch (const std::exception &e) {
			std::cerr << "Interface::getSupported(): " <<
			    e.what() << '\n';
		} catch (...) {
			std::cerr << "Interface::getSupported(): "
			    "Non-standard exception\n";
		}
		break;
	case SlapSegIII::Validation::Operation::Orientation:
		try {
			SlapSegIII::Validation::testOperation(
			    args);
			rv = EXIT_SUCCESS;
		} catch (const std::exception &e) {
			std::cerr << "Interface::determineOrientation(): " <<
			    e.what() << '\n';
		} catch (...) {
			std::cerr << "Interface::determineOrientation(): "
			    "Non-standard exception\n";
		}
		break;
	case SlapSegIII::Validation::Operation::Usage:
		SlapSegIII::Validation::printUsage(argv[0]);
		rv = EXIT_SUCCESS;
		break;
	}

	return (rv);
}
