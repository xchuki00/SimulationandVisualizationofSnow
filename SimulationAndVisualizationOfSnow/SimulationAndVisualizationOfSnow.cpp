// SimulationAndVisualizationOfSnow.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//
#pragma warning(disable: 4482)

#include <iostream>

#include "src/PTPB.hxx"
#include "Bre\EmbreeAcc.hxx"
#include "Misc\Config.hxx"

AbstractRenderer* getPTPB(
	const Config& aConfig,
	const int     aSeed,
	const int     aBaseSeed) {

	const Scene& scene = *aConfig.mScene;
	return new PTPB(
		scene, 
	PTPB::AlgorithmType::kCustom,
		aConfig.mAlgorithmFlags, 
		aConfig.mQueryBeamType,
		aConfig.mBB1DRadiusInitial, 
		aConfig.mBB1DRadiusAlpha, 
		aConfig.mBB1DRadiusCalculation, 
		aConfig.mBB1DRadiusKNN, 
		aConfig.mPhotonBeamType, 
		aConfig.mBB1DUsedLightSubPathCount, 
		aConfig.mBB1DBeamStorageFactor, 
		aConfig.mRefPathCountPerIter, 
		aConfig.mPathCountPerIter,
		aConfig.mMinDistToMed,
		aConfig.mMaxMemoryPerThread, 
		aSeed, aBaseSeed, 
		aConfig.mIgnoreFullySpecPaths
	);

	/*return new PTBP(scene, aSeed, PTBP::kPointBeam2D, aConfig.mPB2DRadiusInitial, aConfig.mPB2DRadiusAlpha, aConfig.mPB2DRadiusCalculation, aConfig.mPB2DRadiusKNN, aConfig.mQueryBeamType,
		aConfig.mBB1DRadiusInitial, aConfig.mBB1DRadiusAlpha, aConfig.mBB1DRadiusCalculation, aConfig.mBB1DRadiusKNN, aConfig.mPhotonBeamType, aConfig.mBB1DUsedLightSubPathCount, aConfig.mRefPathCountPerIter);*/
}

//////////////////////////////////////////////////////////////////////////
// The main rendering function, renders what is in aConfig

float render(
	const Config& aConfig,
	int* oUsedIterations = NULL)
{

	// Don't use more threads than iterations (in case of rendering limited by number of iterations not time)
	int usedThreads = aConfig.mNumThreads;
	if (aConfig.mMaxTime <= 0) usedThreads = std::min(usedThreads, aConfig.mIterations);
	// Set number of used threads
	omp_set_num_threads(usedThreads);

	// Create 1 renderer per thread
	typedef AbstractRenderer* AbstractRendererPtr;
	AbstractRendererPtr* renderers;
	renderers = new AbstractRendererPtr[usedThreads];

	for (int i = 0; i < usedThreads; i++)
	{
		renderers[i] = getPTPB(aConfig, aConfig.mBaseSeed + i, aConfig.mBaseSeed);
		//renderers[i] = CreateRenderer(aConfig, aConfig.mBaseSeed + i, aConfig.mBaseSeed);
		renderers[i]->mMaxPathLength = aConfig.mMaxPathLength;
		renderers[i]->mMinPathLength = aConfig.mMinPathLength;
		renderers[i]->SetupBeamDensity(aConfig.mBeamDensType, aConfig.mScene->mCamera.mResolution, aConfig.mBeamDensMax);
	}

	clock_t startT = clock();
	int iter = 0;

	Framebuffer accumFrameBuffer, outputFrameBuffer;
	accumFrameBuffer.Setup(aConfig.mResolution);
	outputFrameBuffer.Setup(aConfig.mResolution);
	std::string name = aConfig.mOutputName.substr(0, aConfig.mOutputName.length() - 4);
	std::string ext = aConfig.mOutputName.substr(aConfig.mOutputName.length() - 3, 3);
	char filename[1024]; // Must be shared, otherwise critical section fails

	// Rendering loop, when we have any time limit, use time-based loop,
	// otherwise go with required iterations
	if (aConfig.mMaxTime > 0)
	{
		// Time based loop
#pragma omp parallel shared(iter,accumFrameBuffer,outputFrameBuffer,name,ext,filename)
		while (clock() < startT + aConfig.mMaxTime * CLOCKS_PER_SEC)
		{
			int threadId = omp_get_thread_num();
			renderers[threadId]->RunIteration(iter);

#pragma omp critical
			{
				iter++; // counts number of iterations
			}
		}
	}
	else
	{
		// Iterations based loop
		int cnt = 0, p = -1;
#pragma omp parallel for shared(cnt,p,accumFrameBuffer,outputFrameBuffer,name,ext,filename)
		for (iter = 0; iter < aConfig.mIterations; iter++)
		{
			int threadId = omp_get_thread_num();
			renderers[threadId]->RunIteration(iter);
#pragma omp critical
			{

				std::cout << "IT: " << cnt << std::endl;
				++cnt;
				int percent = (int)(((float)cnt / aConfig.mIterations) * 100.0f);
				if (percent != p)
				{
					p = percent;
					std::cout << percent << "%" << std::endl;
				}
			}
		}
		iter = aConfig.mIterations;
	}

	clock_t endT = clock();

	if (oUsedIterations)
		*oUsedIterations = iter + 1;

	int usedRenderers = 0;

	aConfig.mCameraTracingTime = 0;
	aConfig.mBeamDensity.Setup(aConfig.mBeamDensType, aConfig.mScene->mCamera.mResolution, aConfig.mBeamDensMax);

	// Not all created renderers had to have been used.
	// Those must not participate in accumulation.
	for (int i = 0; i < usedThreads; i++)
	{
		
			if (!renderers[i]->WasUsed())
				continue;

			if (aConfig.mContinuousOutput <= 0)
			{
				if (usedRenderers == 0)
				{
					renderers[i]->GetFramebuffer(*aConfig.mFramebuffer);
				}
				else
				{
					Framebuffer tmp;
					renderers[i]->GetFramebuffer(tmp);
					aConfig.mFramebuffer->Add(tmp);
				}
			}

			renderers[i]->AccumulateDebugImages(aConfig.mDebugImages);
			renderers[i]->AccumulateBeamDensity(aConfig.mBeamDensity);

			aConfig.mCameraTracingTime += renderers[i]->mCameraTracingTime;

			usedRenderers++;
		}

		if (aConfig.mContinuousOutput <= 0)
		{
			// Scale framebuffer by the number of used renderers
			aConfig.mFramebuffer->Scale(1.f / usedRenderers);
		}
		else
		{
			*aConfig.mFramebuffer = accumFrameBuffer;
			aConfig.mFramebuffer->Scale(1.f / iter);
		}
	aConfig.mCameraTracingTime /= iter;

	// Clean up renderers
	for (int i = 0; i < usedThreads; i++)
		delete renderers[i];

	delete[] renderers;
	std::cout << "DONE" << std::endl;
	return float(endT - startT) / CLOCKS_PER_SEC;
}
int main(int argc, const char* argv[])
{
	try
	{
		// Warns when not using C++11 Mersenne Twister
		PrintRngWarning();

		EmbreeAcc::initLib();

		// Setups config based on command line
		Config config;
		ParseCommandline(argc, argv, config);

		// If number of threads is invalid, set 1 thread per processor
		if (config.mNumThreads <= 0)
			config.mNumThreads = std::max(1, omp_get_num_procs());

		// When some error has been encountered, exits
		if (config.mScene == NULL)
			return 1;

		// Sets up framebuffer
		Framebuffer fbuffer;
		config.mFramebuffer = &fbuffer;

		// Prints what we are doing
		printf("Scene:    %s\n", config.mScene->mSceneName.c_str());
		if (config.mMaxTime > 0)
			printf("Target:   %g seconds render time\n", config.mMaxTime);
		else
			printf("Target:   %d iteration(s)\n", config.mIterations);

		// Renders the image
		std::string desc = GetDescription(config, "            ");
		printf("Running:  %s", desc.c_str());
		fflush(stdout);
		int iterations;
		float time = render(config, &iterations);
		EmbreeAcc::cleanupLib();
		printf("done in %.2f s (%i iterations)\n", time, (iterations - 1));
		if (config.mCameraTracingTime) printf("avg camera time %.2f s\n", config.mCameraTracingTime);

		std::string extension = config.mOutputName.substr(config.mOutputName.length() - 3, 3);

		if (config.mShowTime || config.mIterations <= 0)
		{
			std::ostringstream modifiedOutputName;

			modifiedOutputName << config.mOutputName.substr(0, config.mOutputName.length() - 4);
			if (config.mIterations <= 0)
			{
				modifiedOutputName << "_i" << iterations - 1;
			}
			if (config.mShowTime)
			{
				modifiedOutputName.precision(2);
				modifiedOutputName << "_time" << std::fixed << time;
			}
			modifiedOutputName << "." << extension;

			config.mOutputName = modifiedOutputName.str();
		}

		// Saves the image
		fbuffer.Save(config.mOutputName, 2.2f /*gamma*/);

		//std::string name = config.mOutputName.substr(0, config.mOutputName.length() - 4);
		//config.mDebugImages.Output(name, extension);
		//config.mBeamDensity.Output(name, extension);

		// Scene cleanup
		delete config.mScene;

		return 0;
	}
	catch (...)
	{
		std::cerr << "Error: unknown error" << std::endl;
		exit(2);
	}
}
