#pragma once

namespace avr_halib{
namespace config{
namespace timer
{
	struct Prescalers
	{
		enum PrescalerType
		{
			noClock,
			ps1,
			ps8,
			ps32,
			ps64,
			ps128,
			ps256,
			ps1024
		};
	};
	typedef Prescalers::PrescalerType PrescalerType;

	struct ExternalClockEdges
	{
		enum ExternalClockEdgeType
		{
			falling,
			rising
		};
	};
	typedef ExternalClockEdges::ExternalClockEdgeType ExternalClockEgdeType;

	struct WaveForms
	{
		enum WaveFormType
		{
			normal,
			PWM,
			clearOnMatch
		};
	};
	typedef WaveForms::WaveFormType WaveFormType;

	struct MatchInputs
	{
		struct MatchInputType
		{
			OCRA,
			ICR
		};
	};
	typedef MatchInputs::MatchInputType;

	struct PWMCorrections
	{
		enum PWMCorrectionType
		{
			none,
			phase,
			frequency,
			phaseAndFrequency
		};
	};
	typedef PWMCorrections::PWMCorrectionType PWMCorrectionType;

	struct PWMAccuracys
	{
		enum PWMAccuracyType
		{
			8Bit,
			9Bit,
			10Bit,
			dynamicOCR,
			dynamicICR
		}
	};
	typedef PWMAccuracys::PWMAccuracyType PWMAccuracyType;

	struct OutputModes
	{
		enum OutputModeType
		{
			noOutput, /**< no output on compare match **/
			toggle,	  /**< toggle output on compare match, not applicable for pwm modes **/
			clear,	  /**< clear output on compare match **/
			set		  /**< set output on compare match **/
		};
	};
	typedef OutputModes::OutputModeType OutputModeType;

	struct MatchModules
	{
		enum MatchModuleType
		{
			matchA,
			matchB,
			matchC
		};
	};
	typedef MatchModules::MatchModuleType MatchModuleType;
}
}
}
