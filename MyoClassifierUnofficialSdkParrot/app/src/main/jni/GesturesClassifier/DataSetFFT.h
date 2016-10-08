#ifndef DATASET_FTT_H
#define DATASET_FTT_H
#pragma once

#include <array>
#include <vector>
#include <complex>
#include "DataSetReader.h"
#include "KissFFT.h"

namespace DataSetFTT
{
	/**
	* Applay FFT to data set
	*/
	static void applay(DataSetReader& data)
	{
		const int N_EMG = 8;
		//type
		using Complex_d = std::complex<double>;
		using FFT_d = KissFF<double>;
		using EmgRowN = std::array< std::vector<Complex_d>, N_EMG >;
		//size of a row
		size_t emg1RowSize = (size_t)data.getFlags().mReps;
		//alloc
		FFT_d fft((int)emg1RowSize, false);
		//
		for (int i = 0; i != data.size(); ++i)
		{
			EmgRowN emgN_samples = data.getEmgFromRow< N_EMG, Complex_d >(i);
			EmgRowN emgN_frq;
			//compute DFT
			for (int i = 0; i != N_EMG; ++i)
			{
				emgN_frq[i].resize(emg1RowSize, { 0,0 });
				fft.transform(emgN_samples[i], emgN_frq[i]);
				fft.normalize(emgN_frq[i]);
			}
			//applay
			data.setAbsEmgToRow< N_EMG, Complex_d >(i, emgN_frq);
		}
	}

	/**
	* Applay FFT to data set row
	*/
	static void applay(DataSetReader::Row& row,const DataFlags& flags)
	{
		const int N_EMG = 8;
		//type
		using Complex_d = std::complex<double>;
		using FFT_d = KissFF<double>;
		using EmgRowN = std::array< std::vector<Complex_d>, N_EMG >;
		//size of a row
		size_t emg1RowSize = (size_t)flags.mReps;
		//alloc
		FFT_d fft((int)emg1RowSize, false);
		//samples
		EmgRowN emgN_samples = DataSetReader::Row::getEmgFromRow< N_EMG, Complex_d >(row, flags);
		EmgRowN emgN_frq;
		//compute DFT
		for (int i = 0; i != N_EMG; ++i)
		{
			emgN_frq[i].resize(emg1RowSize, { 0,0 });
			fft.transform(emgN_samples[i], emgN_frq[i]);
			fft.normalize(emgN_frq[i]);
		}
		//applay
		DataSetReader::Row::setAbsEmgToRow< N_EMG, Complex_d >(row, flags, emgN_frq);
	}

};

#endif