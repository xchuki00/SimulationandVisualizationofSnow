 /*
 * Patrik Chukir
 * xchuki@stud.fit.vutbr.cz
 * p.chukir@gmail.com
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
#include <limits>

 /**
 * @brief	Photon beam.
 */
struct PTPBBeam : PhotonBeam
{

	float startTime = 0;
	int iteration = 0;
	float Rb = 0;
	float mTimeDifference = 0.01f;
	float ior = 1;
	float max = std::numeric_limits<float>::max() / 2;
	Rgb mThroughputAtEnd;
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
	 * @param	rayTime					Start time query ray
	 * @param	additionalDataForMis	(Optional) the additional data for MIS weights computation.
	 */
	__forceinline void accumulate(const Ray& ray, const float mint, const float maxt, const float isectmint, const float isectmaxt, const float beamSelectionPdf, Rgb& accumResult, uint rayFlags, const AbstractMedium* medium, float rayTime,float queryIor, const embree::AdditionalRayDataForMis* additionalDataForMis = NULL)
		const {
		float beamBeamDistance, sinTheta, queryIsectDist, beamIsectDist;
		if (mMedium == medium && testIntersectionBeamBeam(ray.origin, ray.direction, isectmint, isectmaxt, mRay.origin,
			mRay.direction, 0, mLength, mMaxRadiusSqr, beamBeamDistance, sinTheta, queryIsectDist, beamIsectDist))
		{
			if (mStartRadius < beamBeamDistance)
				return;
	
			auto speed = 300000;
			auto rayTimeAtPoit = rayTime + (queryIsectDist / (speed/queryIor));
			auto beamTime = startTime + (beamIsectDist / (speed/ior));
			if ((rayTimeAtPoit - beamTime) > mTimeDifference)
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
			extincion += scattering;
			/*		if (!attenuation.isPositive())
						return;*/
						// Weight and accumulate result.
			//std::cout<<"START: "<< this->mThroughputAtOrigin << ", END: " << this->mThroughputAtOrigin << std::endl;
			float K1D = 1;
			if (beamBeamDistance > 0) {
				K1D/= beamBeamDistance;
			}
			Rgb flux = (this->mThroughputAtOrigin - ((this->mThroughputAtOrigin - this->mThroughputAtEnd) * beamIsectDist) / mLength); //svetelny tok - pocitat pri generovani mapy
			float isoPhaseFunction = 0.07957747154594766788444188168626f; //1/4pi
			float red = flux.r() * isoPhaseFunction * K1D * scattering.r() * ((exp(-extincion.r() * queryIsectDist) * exp(-extincion.r() * beamIsectDist)) / sinTheta);
			float green = flux.g() * isoPhaseFunction * K1D * scattering.g() * ((exp(-extincion.g() * queryIsectDist) * exp(-extincion.g() * beamIsectDist)) / sinTheta);
			float blue = flux.b() * isoPhaseFunction * K1D * scattering.b() * ((exp(-extincion.b() * queryIsectDist) * exp(-extincion.b() * beamIsectDist)) / sinTheta);
			
	/*		if (red >= 10e+20|| green >= 10e+20|| blue >= 10e+20) {
				std::cout<<"COLOR: "<<red<< "= " << flux.r()<<" * " << beamBeamDistance << " * " << scattering.r() << "(( e^ " << -extincion.r() << " * "<<queryIsectDist << ")*( e^ " << -extincion.r() << " * " << beamIsectDist << "))/ " << sinTheta<<", "<< exp(extincion.r() * queryIsectDist) <<", "<< exp(-extincion.r() * beamIsectDist) << std::endl;
			}*/

			//red = (Float::isInf(red)) ? max : red;
			//green = (Float::isInf(green)) ? max : green;
			//blue = (Float::isInf(blue)) ? max : blue;
			//red = (Float::isNan(red)) ? 0.0 : red;
			//green = (Float::isNan(green)) ? 0.0 : green;
			//blue = (Float::isNan(blue))?0.0:blue;
			Rgb coef(red, green, blue);

			accumResult += coef;
			//std::cout << coef << std::endl;
			if (accumResult.isNanInfNeg()) {
				std::cout << this->mThroughputAtOrigin << std::endl;
				std::cout<<"COLOR: "<<accumResult<<"+= "<<red<< ", " << green<<", " << blue<< std::endl;
				std::cout << "COLOR: " << blue << "= " << flux.b() << " * " << K1D << " * " << scattering.b() << "(( e^ " << -extincion.b() << " * " << queryIsectDist << ")*( e^ " << -extincion.b() << " * " << beamIsectDist << "))/ " << sinTheta << ", " << exp(extincion.b() * queryIsectDist) << ", " << exp(-extincion.b() * beamIsectDist) << std::endl;

			}
			//std::cout << "COLOR: " << accumResult << "; " << std::endl;
			UPBP_ASSERT(!accumResult.isNanInfNeg());
		}
	}

};

/**
 * @brief	Defines an alias representing array of photon beams.
 */
typedef std::vector<PTPBBeam> PTPBBeamsArray;

#endif //__PTPBBEAM_HXX__