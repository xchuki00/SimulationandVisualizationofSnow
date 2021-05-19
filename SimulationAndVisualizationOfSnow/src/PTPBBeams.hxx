/*
 * Copyright (C) 2014, Petr Vevoda, Martin Sik (http://cgg.mff.cuni.cz/~sik/),
 * Tomas Davidovic (http://www.davidovic.cz), Iliyan Georgiev (http://www.iliyan.com/),
 * Jaroslav Krivanek (http://cgg.mff.cuni.cz/~jaroslav/)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * (The above is MIT License: http://en.wikipedia.origin/wiki/MIT_License)
 *
 *
 *  Patrik Chukir
 * xchuki@stud.fit.vutbr.cz
 * p.chukir@gmail.com
 * Tento kód je pøevzatý ze smallUpbp a upravený pro potøeby metody Progressive transient photon beams.
 * Vzhledem k tomu, že úzce využívá funkcionalitu SmallUPBP, je zde velká míra shodných èástí kódu.
 */

#ifndef __PTPBBEAMS_HXX__
#define __PTPBBEAMS_HXX__

/**
 * @brief	Use one of the following symbols to set acceleration structure for faster beam
 * 			intersection: USE_BRUTE (slow), USE_EMBREE (currently does not work), USE_GRID (recommended).
 */
#define USE_GRID

#include "PTPBBeam.hxx"
#include "Beams\GridStats.hxx"
#include "Path\PhaseFunction.hxx"
#include "Scene\Scene.hxx"
#include "PTPBGrid.hxx"
/**
 * @brief	Support for photon beams estimate.
 */
class PTPBEvaluator
{
public:

	/**
	 * @brief	Constructor.
	 *
	 * @param	aScene	The scene the beams are located in (needed for access to media).
	 */
	PTPBEvaluator(const Scene& aScene)
		: scene(aScene), mSeed(1234)
	{
		accelStruct = nullptr;
	}

	/**
	 * @brief	Destructor.
	 * 			
	 * 			Destroys the used acceleration structure.
	 */
	~PTPBEvaluator()
	{
		destroy();
	}

	/**
	 * @brief	Builds the data structure for Beam-Beam queries on the given beams.
	 *
	 * @param [in,out]	beams	 	The beams.
	 * @param	radiusCalculation	Type of radius calculation.
	 * @param	beamRadius		 	Beam radius.
	 * @param	knn				 	Value x means that x-th closest beam vertex will be used for
	 * 								calculation of cone radius at the current beam vertex.
	 * @param	verbose			 	Whether to print information about progress.
	 */
	void build(
		PTPBBeamsArray & beams,
		RadiusCalculation radiusCalculation,
		const float beamRadius,
		const int knn,
		bool verbose);

	/**
	 * @brief	Destroys the data structure for Beam-Beam queries.
	 */
	void destroy();

	/**
	 * @brief	Evaluates the beam-beam estimate for the given query ray.
	 *
	 * @param	beamType					   	Type of the beam.
	 * @param	queryRay					   	The query ray (=beam) for the Beam-beam estimate.
	 * @param	segments					   	Full volume segments of media intersected by the ray.
	 * @param	rayTime							Start time of query ray
	 * @param	estimatorTechniques			   	(Optional) the estimator techniques to use.
	 * @param	raySamplingFlags			   	(Optional) the ray sampling flags (\c AbstractMedium::kOriginInMedium).
	 * @param [in,out]	additionalRayDataForMis	(Optional) additional data needed for MIS weights
	 * 											computations.
	 * @param [in,out]	gridStats			   	(Optional) statistics to gather for the ray.
	 *
	 * @return	The accumulated radiance along the ray.
	 */
	Rgb evalBeamBeamEstimate(
		BeamType beamType,
		const Ray& queryRay,
		const VolumeSegments& segments,
		float rayTime,
		float ior,
		const uint estimatorTechniques = BB1D,
		const uint raySamplingFlags = 0,
		embree::AdditionalRayDataForMis* additionalRayDataForMis = NULL,
		GridStats* gridStats = NULL
		);

	Rgb evalBeamBeamEstimate(
		BeamType beamType,
		const Ray& queryRay,
		const LiteVolumeSegments& segments,
		float rayTime,
		float ior,
		const uint estimatorTechniques,
		const uint raySamplingFlags,
		embree::AdditionalRayDataForMis* additionalRayDataForMis = NULL,
		GridStats* gridStats = NULL
	);

	/**
	 * @brief	Gets probability of selecting a beam (in case of beam reduction) around the given
	 * 			position.
	 *
	 * @param	pos	The position.
	 *
	 * @return	The beam selection PDF.
	 */
	float getBeamSelectionPdf(const Pos & pos) const;

	static uint sGridSize;       //!< Size of the grid.
	static uint sMaxBeamsInCell; //!< Maximum number of tested beams in a single cell.
	static uint sReductionType;  //!< Type of the reduction of numbers of tested beams in cells.	
	
	int mSeed; //!< Seed for sampling beams during the reduction.

private:
	const Scene& scene;        //!< Reference to the scene for material evaluation.
	PTPBAccelStruct * accelStruct; //!< Acceleration structure for beams.
};


#endif // __EMBREEACC_HXX__
