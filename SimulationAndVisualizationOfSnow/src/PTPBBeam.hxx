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
 */

#ifndef __PTPBBEAM_HXX__
#define __PTPBBEAM_HXX__

#include <vector>

#include "common\ray.h"
#include "Misc\Debugimages.hxx"
#include "Structs\BoundingBox.hxx"
#include "Beams/PhotonBeam.hxx"
#include "Path/PathWeight.hxx"
#include <cmath>
#include <math.h>  

 /**
 * @brief	Photon beam.
 */
struct PTPBBeam : PhotonBeam
{

	float mDistance = 0;
	int iteration = 0;
	float flush = 0;
	float Rb = 0;
	float mTimeDifference = 0.1;
	/**
	 * @brief	Accumulates photon beam contribution to ray with given [mint,maxt] and flags,
	 * 			accumulation is stored to \c accumResult.
	 *
	 * 			Version used by pure BB1D algorithm from \c VolLightTracer.hxx.
	 *
	 * @param	ray						The query ray.
	 * @param	mint					Original minimum value of the ray t parameter.
	 * @param	maxt					Original maximum value of the ray t parameter.
	 * @param	isectmint				Minimum value of the ray t parameter inside the tested cell.
	 * @param	isectmaxt				Maximum value of the ray t parameter inside the tested cell.
	 * @param	beamSelectionPdf		PDF of testing a beam in the tested cell.
	 * @param [in,out]	accumResult 	Accumulated result.
	 * @param	rayFlags				Ray flags (beam type and estimator techniques).
	 * @param	medium					Medium the current ray segment is in.
	 * @param	additionalDataForMis	(Optional) the additional data for MIS weights computation.
	 */
	__forceinline void accumulate(const Ray& ray, const float mint, const float maxt, const float isectmint, const float isectmaxt, const float beamSelectionPdf, Rgb& accumResult, uint rayFlags, const AbstractMedium* medium, float rayTime, const embree::AdditionalRayDataForMis* additionalDataForMis = NULL)
		const {
		if (additionalDataForMis)
		{
			accumulate2(ray, mint, maxt, isectmint, isectmaxt, beamSelectionPdf, accumResult, rayFlags, medium,rayTime, additionalDataForMis);
			return;
		}
		float beamBeamDistance, sinTheta, queryIsectDist, beamIsectDist;
	
		if (mMedium == medium && testIntersectionBeamBeam(ray.origin, ray.direction, isectmint, isectmaxt, mRay.origin,
			mRay.direction, 0, mLength, mMaxRadiusSqr, beamBeamDistance, sinTheta, queryIsectDist, beamIsectDist))
		{
			if (mStartRadius < beamBeamDistance)
				return;

			auto distance = beamIsectDist;
			auto speed = 300000;
			auto beamTime = mDistance + (distance / speed);

			if ((rayTime - beamTime) > mTimeDifference)
				return;
			Rgb attenuation, scattering, extincion;

			if (medium->IsHomogeneous())
			{
				const HomogeneousMedium* hmedium = ((const HomogeneousMedium*)medium);
				scattering = hmedium->GetScatteringCoef();
				extincion = hmedium->GetAbsorptionCoef();
			}
			else
			{
				scattering = medium->GetScatteringCoef(ray.target(queryIsectDist));
				extincion = medium->GetAbsorptionCoef(ray.target(queryIsectDist));

			}
			/*		if (!attenuation.isPositive())
						return;*/
						// Weight and accumulate result.
			float red = beamBeamDistance * scattering.r() * ((exp(extincion.r() * queryIsectDist) * exp(-extincion.r() * beamIsectDist)) / sinTheta);
			float green = beamBeamDistance * scattering.g() * ((exp(extincion.g() * queryIsectDist) * exp(-extincion.g() * beamIsectDist)) / sinTheta);
			float blue = beamBeamDistance * scattering.b() * ((exp(-extincion.b() * queryIsectDist) * exp(-extincion.b() * beamIsectDist)) / sinTheta);
			Rgb coef(red, green, blue);
			accumResult += mThroughputAtOrigin * coef;
			/*		misWeight *
					unweightedResult;*/

			UPBP_ASSERT(!accumResult.isNanInfNeg());
		}
	}

	/**
	 * @brief	Accumulates photon beam contribution to ray with given [mint,maxt] and flags,
	 * 			accumulation is stored to \c accumResult.
	 * 			
	 * 			Version used by combined BB1D algorithms from \c UPBP.hxx.
	 *
	 * @param	ray						The query ray.
	 * @param	mint					Original minimum value of the ray t parameter.
	 * @param	maxt					Original maximum value of the ray t parameter.
	 * @param	isectmint				Minimum value of the ray t parameter inside the tested cell.
	 * @param	isectmaxt				Maximum value of the ray t parameter inside the tested cell.
	 * @param	beamSelectionPdf		PDF of testing a beam in the tested cell.
	 * @param [in,out]	accumResult 	Accumulated result.
	 * @param	rayFlags				Ray flags (beam type and estimator techniques).
	 * @param	medium					Medium the current ray segment is in.
	 * @param	additionalDataForMis	(Optional) the additional data for MIS weights computation.
	 */
	__forceinline void accumulate2(const Ray &ray, const float mint, const float maxt, const float isectmint, const float isectmaxt, const float beamSelectionPdf, Rgb & accumResult, uint rayFlags, const AbstractMedium * medium,float rayTime,const embree::AdditionalRayDataForMis* additionalDataForMis = NULL) const 
	{				
		float beamBeamDistance, sinTheta, queryIsectDist, beamIsectDist;
		if (mMedium == medium && testIntersectionBeamBeam(ray.origin, ray.direction, isectmint, isectmaxt, mRay.origin,
			mRay.direction, 0, mLength, mMaxRadiusSqr, beamBeamDistance, sinTheta, queryIsectDist, beamIsectDist))
		{
			if (mStartRadius < beamBeamDistance)
				return;

			auto distance =  beamIsectDist;
			auto speed = 300000;
			auto beamTime = mDistance + (distance / speed);

			if ((rayTime - beamTime)>mTimeDifference)
				return;
			Rgb attenuation, scattering, extincion;

			if (medium->IsHomogeneous())
			{
				const HomogeneousMedium * hmedium = ((const HomogeneousMedium *)medium);
				scattering = hmedium->GetScatteringCoef();
				extincion = hmedium->GetAbsorptionCoef();
			}
			else
			{
				scattering = medium->GetScatteringCoef(ray.target(queryIsectDist));
				extincion = medium->GetAbsorptionCoef(ray.target(queryIsectDist));

			}
	/*		if (!attenuation.isPositive())
				return;*/
			// Weight and accumulate result.
			float red = beamBeamDistance * scattering.r() * ((exp(extincion.r() * queryIsectDist) * exp(-extincion.r() * beamIsectDist)) / sinTheta);
			float green = beamBeamDistance * scattering.g() * ((exp(extincion.g() * queryIsectDist) * exp(-extincion.g() * beamIsectDist)) / sinTheta);
			float blue = beamBeamDistance * scattering.b() * ((exp(-extincion.b() * queryIsectDist) * exp(-extincion.b() * beamIsectDist)) / sinTheta);
			Rgb coef(red,green,blue);
			accumResult += mThroughputAtOrigin * coef;
			UPBP_ASSERT(!accumResult.isNanInfNeg());
		}
	}
};

/**
 * @brief	Defines an alias representing array of photon beams.
 */
typedef std::vector<PTPBBeam> PTPBBeamsArray;

#endif //__PTPBBEAM_HXX__