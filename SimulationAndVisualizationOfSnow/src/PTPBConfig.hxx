/*
 *  Patrik Chukir
 * xchuki@stud.fit.vutbr.cz
 * p.chukir@gmail.com
 * Tento kód je pøevzatý ze smallUpbp a upravený pro potøeby metody Progressive transient photon beams.
 * Vzhledem k tomu, že úzce využívá funkcionalitu SmallUPBP, je zde velká míra shodných èástí kódu.
 */

#ifndef __PTPBCONFIG_HXX__
#define __PTPBCONFIG_HXX__

#include "Misc/Config.hxx"
#include "PTPB.hxx"

 /**
 * @brief	Photon beam.
 */
struct PTPBConfig : Config
{

	enum Algorithm
	{
		ProgressiveTrainsientPhotonBeams
	};
		/**
		 * @brief	Gets a name of the given algorithm.
		 *
		 * 			The name is the human readable name of an algorithm displayed in the help.
		 *
		 * @param	aAlgorithm	The algorithm.
		 *
		 * @return	"unknown algorithm" if the value specified is not from \c Algorithm enum, otherwise
		 * 			its name.
		 */
		static const char* GetName(Algorithm aAlgorithm)
		{
			static const char* algorithmNames[kAlgorithmMax] =
			{
				"Progressive Trainsient Photon Beams"
			};

			if (aAlgorithm < 0 || aAlgorithm >= kAlgorithmMax)
				return "unknown algorithm";

			return algorithmNames[aAlgorithm];
		}

		/**
		 * @brief	Gets an acronym of the given algorithm.
		 *
		 * @param	aAlgorithm	The algorithm.
		 *
		 * 						The acronym is the short code for an algorithm displayed in the help and
		 * 						used as an argument of -a option.
		 *
		 * @return	"unknown" if the value specified is not from \c Algorithm enum, otherwise its acronym.
		 */
		static const char* GetAcronym(Algorithm aAlgorithm)
		{
			static const char* algorithmNames[kAlgorithmMax] = {
				"ptpb"};

			if (aAlgorithm < 0 || aAlgorithm >= kAlgorithmMax)
				return "unknown";

			return algorithmNames[aAlgorithm];
		}
		float mPTPB1DRadiusInitial = -0.001f;
		float mPTPB1DRadiusAlpha = 1.0f;
		RadiusCalculation mPTPB1DRadiusCalculation;     //!< Type of photon beam radius calculation.	
		float             mPTPB1DUsedLightSubPathCount; //!< Number of light paths used to generate photon beams.
		float             mPTPB1DBeamStorageFactor;     //!< Factor used for computation of minimum MFP of media where photon beams are used. The lower it is the denser media will use photon beams.	


	};
	
	/**
	 * @brief	Utility function, essentially a renderer factory.
	 *
	 * 			Creates a renderer according to \c aConfig.mAlgorithm with all relevant settings
	 * 			stored in \c aConfig.
	 *
	 * @param	aConfig  	The configuration for the renderer.
	 * @param	aSeed	 	Thread specific seed. Used by all renderers but \c UPBP.
	 * @param	aBaseSeed	Global seed. Used by \c UPBP only.
	 *
	 * @return	New renderer that corresponds to settings in the given \c aConfig.
	 */
	AbstractRenderer* CreateRenderer(
		const PTPBConfig& aConfig,
		const int     aSeed,
		const int     aBaseSeed)
	{
		const Scene& scene = *aConfig.mScene;
		return new PTPB(
			scene,
			PTPB::AlgorithmType::kPTPB,
			aConfig.mAlgorithmFlags,
			aConfig.mQueryBeamType,
			aConfig.mPTPB1DRadiusInitial,
			aConfig.mPTPB1DRadiusCalculation,
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
	}

	/**
	 * @brief	Gets a description of the given configuration.
	 *
	 * 			Description is a multi line string that summarizes all settings in the given \c Config
	 * 			relevant to the current rendering. It is displayed at the beginning of the rendering.
	 *
	 * @param	aConfig		  	The configuration to describe.
	 * @param	aLeadingSpaces	Leading spaces to indent the description.
	 *
	 * @return	The description of the given configuration.
	 */
	std::string GetDescription(const PTPBConfig& aConfig, const std::string& aLeadingSpaces)
	{
			std::ostringstream oss;
			oss << aConfig.GetName(PTPBConfig::Algorithm::ProgressiveTrainsientPhotonBeams) << '\n'
				<< aLeadingSpaces << "resolution:        " << aConfig.mResolution.x << "x" << aConfig.mResolution.y << '\n'
				<< aLeadingSpaces << "max path length:   " << aConfig.mMaxPathLength << '\n'
				<< aLeadingSpaces << "bb1d radius init:  " << aConfig.mBB1DRadiusInitial << '\n'
				<< aLeadingSpaces << "     radius alpha: " << aConfig.mBB1DRadiusAlpha << '\n';

			if (aConfig.mBB1DRadiusCalculation == KNN_RADIUS)
				oss << aLeadingSpaces << "     radius knn:   " << aConfig.mBB1DRadiusKNN << '\n';

			oss << aLeadingSpaces << "     used l.paths: " << aConfig.mBB1DUsedLightSubPathCount << '\n';

			if (aConfig.mQueryBeamType == SHORT_BEAM)
				oss << aLeadingSpaces << "query beam type:   SHORT\n";
			else
				oss << aLeadingSpaces << "query beam type:   LONG\n";

			if (aConfig.mPhotonBeamType == SHORT_BEAM)
				oss << aLeadingSpaces << "photon beam type:  SHORT\n";
			else
				oss << aLeadingSpaces << "photon beam type:  LONG\n";

			if (aConfig.mMaxBeamsInCell > 0)
			{
				oss << aLeadingSpaces << "max beams/cell:    " << aConfig.mMaxBeamsInCell << '\n';
				oss << aLeadingSpaces << "reduction type:    " << aConfig.mReductionType << '\n';
			}

			oss << aLeadingSpaces << "ref. paths/iter:   " << aConfig.mRefPathCountPerIter << '\n';

			return oss.str();
		
	}

	/**
	 * @brief	Prints a full help.
	 *
	 * 			Full help contains all available options and lists all algorithms and predefined
	 * 			scenes.
	 *
	 * @param	argv	Arguments passed on the command line. Only a name of the exe file is used.
	 */
	void PrintPTPBHelp(const char* argv[])
	{
		printf("\n");
		printf("Usage: %s <options>\n\n", argv[0]);
		printf("    Basic options:\n\n");

		printf("    -s  <scene_id> Selects the scene (default 0):\n");
		printf("        -1    user must supply additional argument which contains path to an obj scene file  \n");
		for (int i = 0; i < g_SceneConfigs.size(); i++)
			printf("        %-2d    %s\n", i, g_SceneConfigs[i].mLongName.c_str());

		printf("\n");

		printf("    -l <length>    Maximum length of traced paths (default 10).\n");
		printf("    -t <sec>       Number of seconds to run the algorithm.\n");
		printf("    -i <iter>      Number of iterations to run the algorithm (default 1).\n");
		printf("    -o <name>      User specified output name, with extension .bmp or .exr (default .exr). The name can be prefixed with relative or absolute path but the path must exists.\n");
		printf("    -r <res>       Image resolution in format WIDTHxHEIGHT (default 256x256).\n");
		printf("    -seed <seed>   Sets base seed (default 1234).\n");
		printf("\n    Note: Time (-t) takes precedence over iterations (-i) if both are defined.\n");

		printf("\n    Performance options:\n\n");
		printf("    -th <threads>                     Number of threads (default 0 means #threads = #cores).\n");
		printf("    -maxMemPerThread <memory>         Sets max memory in MB for light vertex array per each thread (default 500). Works only for upbp algorithms.\n");

		printf("\n    Radius options:\n\n");
		printf("    -r_alpha_ptpb1d <alpha>  Sets radius reduction parameter for technique bb1d (default 1, value of 1 implies no radius reduction).\n");
		printf("    -r_initial_ptpb1d_knn <const> <knn>  Makes photon beam's conic with radius at each end computed from distance between beam end and <knn>th closest beam vertex multiplied by <const>.\n");
		
		printf("\n    Beams options:\n\n");
		printf("    -gridres <res>          Sets photon beams grid resolution in dimension of a maximum extent of grid AABB, resolution in other dim. is set to give cube sized grid cells (default 256).\n");
		printf("    -gridmax <max>          Sets maximum number of beams in one grid cell (default 0 means unlimited). Works only for bb1d algorithm (not upbp).\n");
		printf("    -gridred <red>          Sets type of reduction of tested beams in one grid cell (0=presample (default), 1=offset, 2=resample_fixed, 3=resample). Works only for bb1d algorithm (not upbp).\n");
		printf("    -beamdens <type> <max>  Outputs image(s) of statistics of hit beams and cells (0=none(default), 1=abs, 2=avg, 3=cells, 4=overfull, 5=all) normalized to the given max (-1=max in data(default), positive=given max). Works only for bb1d algorithm (not upbp).\n");
		printf("    -beamstore <factor>     Sets multiple of bb1d radius used for decision whether to store beams or not (0=stores all beams (default)). Works only for bb1d algorithm (not upbp).\n");
		printf("    -qbt <type>             Sets query beam type: S = uses short query beams,  L = uses long query beams (default).\n");
		printf("    -pbt <type>             Sets photon beam type: S = uses short photon beams (default), L = uses long photon beams.\n");
		printf("    -pbc <count>            First <count> traced light paths will generate photon beams (default -1, if positive, absolute, if negative, relative to total number of pixels).\n");
		printf("    -nosin                  Use sine only to compute bb1d contribution not weights.\n");
		printf("    -time                            If present algorithm run duration is appended to the name of the output file.\n");
	}
	void PrintShortPTPBHelp(const char* argv[])
	{
		printf("\n");
		printf("Usage: %s [ -s <scene_id> | -a <algorithm> | -l <path length> |\n", argv[0]);
		printf("           -t <time> | -i <iteration> | -o <output_name> ]\n\n");

		printf("    -s  Selects the scene (default 0):\n");
		printf("          -1    user must supply additional argument which contains path to an obj scene file  \n");
		for (int i = 0; i < 5; i++)
			printf("          %-2d    %s\n", i, g_SceneConfigs[i].mLongName.c_str());
		printf("          5..%zd other predefined simple scenes, for complete list, please see full help (-hf)\n", g_SceneConfigs.size() - 1);

		printf("          for complete list, please see full help (-hf)\n");

		printf("    -l  Maximum length of traced paths (default 10).\n");
		printf("    -t  Number of seconds to run the algorithm.\n");
		printf("    -i  Number of iterations to run the algorithm (default 1).\n");
		printf("    -o  User specified output name, with extension .bmp or .exr (default .exr). The name can be prefixed with relative or absolute path but the path must exists.\n");
		printf("\n    Note: Time (-t) takes precedence over iterations (-i) if both are defined.\n");
		printf("\n    For more options, please see full help (-hf)\n");
	}
	/**
	 * @brief	Parses command line and sets up the \c Config according to it.
	 *
	 * @param	argc		   	Number of command line arguments.
	 * @param	argv		   	The command line arguments.
	 * @param [in,out]	oConfig	The configuration to set.
	 */
	void PTPBParseCommandline(int argc, const char* argv[], PTPBConfig& oConfig)
	{
		oConfig.mPTPB1DRadiusInitial = -0.001f;
		oConfig.mPTPB1DRadiusAlpha = 1.0f;
		for (int i = 1; i < argc; i++)
		{
			std::string arg(argv[i]);
			if (arg == "-h" || arg == "--help" || arg == "/?")
			{
				PrintShortPTPBHelp(argv);
				return;
			}
			else if (arg == "-hf" || arg == "--help_full" || arg == "/??")
			{
				PrintPTPBHelp(argv);
				return;
			}
			if (arg == "-r_initial_ptpb1d") // initial radius for PTPB
			{
				if (++i == argc) ReportParsingError("missing argument of -r_initial_bb1d option, please see help (-hf)");

				float init;
				sscanf_s(argv[i], "%f", &init);
				if (init == 0.0f) ReportParsingError("invalid argument of -r_initial_bb1d option, please see help (-hf)");
					oConfig.mPTPB1DRadiusInitial = init;
					oConfig.mPTPB1DRadiusCalculation = CONSTANT_RADIUS;

			}
			else if (arg == "-r_alpha_bb1d") // radius reduction factor for BB1D
			{
				if (++i == argc) ReportParsingError("missing argument of -r_alpha_bb1d option, please see help (-hf)");

				sscanf_s(argv[i], "%f", &oConfig.mPTPB1DRadiusAlpha);
				if (oConfig.mPTPB1DRadiusAlpha <= 0.0f) ReportParsingError("invalid argument of -r_alpha_bb1d option, please see help (-hf)");

			}
		}
		ParseCommandline(argc, argv, oConfig);
		oConfig.mAlgorithmFlags |= BB1D;
		oConfig.mPTPB1DUsedLightSubPathCount = oConfig.mBB1DUsedLightSubPathCount;
		oConfig.mPTPB1DBeamStorageFactor = oConfig.mBB1DBeamStorageFactor;
		oConfig.mPTPB1DRadiusCalculation = CONSTANT_RADIUS;
		PTPBEvaluator::sGridSize = oConfig.mGridResolution;
		PTPBEvaluator::sMaxBeamsInCell = oConfig.mMaxBeamsInCell;
		PTPBEvaluator::sReductionType = oConfig.mReductionType;

	}



#endif //__PTPBCONFIG_HXX__