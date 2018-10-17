/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#ifndef SLAPSEGIII_NULLIMPL_H_
#define SLAPSEGIII_NULLIMPL_H_

#include <slapsegiii.h>

namespace SlapSegIII
{
	class NullImplementation : public Interface
	{
	public:
		SubmissionIdentification
		getIdentification()
		    const
		    override;

		std::set<SlapImage::Kind>
		getSupported()
		    const
		    override;

		std::tuple<ReturnStatus, std::vector<SegmentationPosition>>
		segment(
		    const SlapImage &image)
		    override;

		~NullImplementation() = default;
	};
}

#endif /* SLAPSEGIII_NULLIMPL_H_ */
