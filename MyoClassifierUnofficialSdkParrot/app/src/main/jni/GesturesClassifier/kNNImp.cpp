//
//  kNNImp.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 04/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#include <climits>
#include "DataSetFFT.h"
#include "kNNImp.h"
#include "kNN/kNN.h"
#include <cstring>
#include <cctype>
#include <complex>
#include <cstdlib>

static const char* kNNDistanceTypeToStr[] =
{
	"EUCLIDE_DISTANCE",
	"MANHATTAN_DISTANCE",
	"HAMMING_DISTANCE"
};

static const char* kNNDistanceWeightToStr[] =
{ 
	"DEMOCRATIC",
	"ONE_ON_DISTANCE",
	"ONE_MINUS_DISTANCE"
};

class MyoModelkNN : public MyoModelInterface
{
public:
	//version
	const int cFileModelVersion = 2;
	//
	int mMinRowToGetClass { 5 };  //< the minimum number of records to classify a class
	int mMinRowPerClass   { 1 };  //< the minimum number of records per class
	int mUseFFT	      { false };  //< use FFT
    DataSetReader mData;
    std::unique_ptr<kNN> mModel    { nullptr };
	kNN::DistanceType mDistType    { kNN::EUCLIDE_DISTANCE };
	kNN::DistanceWeight mDistWeight{ kNN::ONE_ON_DISTANCE };
    
    MyoModelkNN(kNN* model = nullptr)
    :mModel(model)
    {
    }
    
    MyoModelkNN(const DataSetReader& data)
	:MyoModelkNN(data,"")
    {
    }

	MyoModelkNN(const DataSetReader& data,const std::string& args)
	{
		//parse args
		parseArgs(args);
		//sava dataset
		mData = data;
		//applay FFT
		if (mUseFFT) applayFFTtoDataSet();
		//map of count row per class
		std::map< double, int > mapCount;
		//count..
		for (auto it : data)
		{
			mapCount[it.mClass] += 1;
		}
		//serach min
		mMinRowPerClass = INT_MAX;
		for (auto it : mapCount)
		{
			mMinRowPerClass = std::max(1, std::min(it.second, mMinRowPerClass));
		}
		//create model
		mModel = std::unique_ptr<kNN>(new kNN(data.getRows()));
	}
    
    virtual ~MyoModelkNN()
    {
    }
   
    virtual void serialize(const std::string& path)
    {
        FILE* file = std::fopen(path.c_str(), "wb");
        
        if(file)
        {
			fprintf(file, "kNN: %d\n", cFileModelVersion);
			fprintf(file, "MinRowToGetClass: %d\n", mMinRowToGetClass);
			fprintf(file, "MinRowPerClass: %d\n", mMinRowPerClass);
			fprintf(file, "UseFFT: %d\n", mUseFFT);
			fprintf(file, "DistanceType: %s\n", kNNDistanceTypeToStr[mDistType]);
			fprintf(file, "DistanceWeight: %s\n", kNNDistanceWeightToStr[mDistWeight]);
            mData.serialize(file);
            fclose(file);
        }
    }
    
    virtual void deserialize(const std::string& path)
    {
        FILE* file = std::fopen(path.c_str(), "rb");
        
        if(file)
        {
			int fileModelVersion = 1;
			mMinRowToGetClass = 5;
			mMinRowPerClass   = 1;
			mDistType         = kNN::EUCLIDE_DISTANCE;
			mDistWeight       = kNN::ONE_ON_DISTANCE;
			mUseFFT			  = false;
			//get version
			fscanf(file, "kNN: %d\n", &fileModelVersion);
			//read output
			if (fileModelVersion == 2)
			{
				//int value
				fscanf(file, "MinRowToGetClass: %d\n", &mMinRowToGetClass);
				fscanf(file, "MinRowPerClass: %d\n", &mMinRowPerClass);
				fscanf(file, "UseFFT: %d\n", &mUseFFT);
				//string types
				char bufferDistanceType[255];
				//fscanf_s(file, "DistanceType: %s\n", bufferDistanceType, 255);
				fscanf(file, "DistanceType: %s\n", bufferDistanceType);
				char bufferDistanceWeight[255];
				//fscanf_s(file, "DistanceWeight: %s\n", bufferDistanceWeight, 255);
				fscanf(file, "DistanceWeight: %s\n", bufferDistanceWeight);
				//get values
				setDistanceTypeFromString(bufferDistanceType);
				setDistanceWeightFromString(bufferDistanceType);
			}
			else
			{
				fscanf(file, "MinRowPerClass: %d\n", &mMinRowPerClass);
			}
            //deserialize
            mData.deserialize(file);
            //close file
            fclose(file);
            //create model
            mModel = std::unique_ptr<kNN>(new kNN(mData.getRows()));
        }
    }

	/*!
	* \brief classify execute the classification
	* \param values
	*/
	kNN::Result classify(DataSetReader::Row& row) const
	{
		//applay fft
		if (mUseFFT)
		{
			applayFFTtoRow(row);
		}
		//classify
		return mModel->classify(row.mRaw,
								std::min(mMinRowToGetClass, mMinRowPerClass),
								mDistType,
								mDistWeight);

	}

	/**
	* Applay FFT to data set
	*/
	void applayFFTtoDataSet()
	{
		DataSetFTT::applay(mData);
	}

	/**
	* Applay FFT to data set row
	*/
	void applayFFTtoRow(DataSetReader::Row& row) const
	{
		DataSetFTT::applay(row, mData.getFlags());
	}

	//args parser
	bool compare(const char* ptr, const char* command)
	{
		return compare(ptr, command, strlen(command));
	}

	bool compare(const char* ptr, const char* command, size_t len)
	{
		return std::strncmp(ptr, command, len) == 0;
	}

	bool compareAndSkip(const char*& ptr, const char* command)
	{
		//string len
		size_t len = strlen(command);
		//compare..
		if (compare(ptr, command, len))
		{
			ptr += len;
			return true;
		}
		return  false;
	}

	void skipSpace(const char*& ptr)
	{
		while (std::isspace(*ptr)) ++ptr;
	}

	int parseIntAndSkip(const char*& ptr)
	{
		//compute end
		const char* ptr_end = ptr;
		//get all digits
		while (isdigit(*ptr_end)) ++ptr_end;
		//isn't int number?
		if (ptr == ptr_end) return 0;
		//parse
		int output = std::atoi(ptr);
		//skip
		ptr = ptr_end;
		//return value
		return output;
	}

	bool parseBoolAndSkip(const char*& ptr)
	{
		//is false?
		if (compareAndSkip(ptr, "false")) return false;
		//skip true
		compareAndSkip(ptr, "true");
		//return true
		return true;
	}

	void parseArgs(const std::string& args)
	{
		//get ptr
		const char* ptr = args.c_str();
		//parse
		while (ptr && *ptr != EOF && *ptr != '\0')
		{
			//skip start space
			skipSpace(ptr);
			//type...
			if (compareAndSkip(ptr, "type"))
			{
				skipSpace(ptr);
				//types
				     if (compareAndSkip(ptr, "EUCLIDE_DISTANCE"))		 mDistType = kNN::EUCLIDE_DISTANCE;
				else if (compareAndSkip(ptr, "MANHATTAN_DISTANCE"))		 mDistType = kNN::MANHATTAN_DISTANCE;
				else if (compareAndSkip(ptr, "HAMMING_DISTANCE"))		 mDistType = kNN::HAMMING_DISTANCE;
				else break;
			}
			else if (compareAndSkip(ptr, "weight"))
			{
				skipSpace(ptr);
				//types
				     if (compareAndSkip(ptr, "DEMOCRATIC"))		         mDistWeight = kNN::DEMOCRATIC;
				else if (compareAndSkip(ptr, "ONE_MINUS_DISTANCE"))		 mDistWeight = kNN::ONE_MINUS_DISTANCE;
				else if (compareAndSkip(ptr, "ONE_ON_DISTANCE"))		 mDistWeight = kNN::ONE_ON_DISTANCE;
				else break;
			}
			else if (compareAndSkip(ptr, "records_to_classify") || compareAndSkip(ptr, "rtoc"))
			{
				skipSpace(ptr);
				//types
				mMinRowToGetClass = parseIntAndSkip(ptr);
			}
			else if (compareAndSkip(ptr, "fft") || compareAndSkip(ptr, "dft"))
			{
				skipSpace(ptr);
				//types
				mUseFFT = parseBoolAndSkip(ptr);
			}
			else break;
		}
	}

	void setDistanceTypeFromString(const std::string& args)
	{
		if (std::strcmp(args.c_str(), kNNDistanceTypeToStr[kNN::EUCLIDE_DISTANCE]) == 0)
			mDistType = kNN::EUCLIDE_DISTANCE;
		else if (std::strcmp(args.c_str(), kNNDistanceTypeToStr[kNN::MANHATTAN_DISTANCE]) == 0)
			mDistType = kNN::MANHATTAN_DISTANCE;
		else if (std::strcmp(args.c_str(), kNNDistanceTypeToStr[kNN::HAMMING_DISTANCE]) == 0)
			mDistType = kNN::HAMMING_DISTANCE;
	}

	void setDistanceWeightFromString(const std::string& args)
	{
		if (std::strcmp(args.c_str(), kNNDistanceWeightToStr[kNN::DEMOCRATIC]) == 0)
			mDistWeight = kNN::DEMOCRATIC;
		else if (std::strcmp(args.c_str(), kNNDistanceWeightToStr[kNN::ONE_ON_DISTANCE]) == 0)
			mDistWeight = kNN::ONE_ON_DISTANCE;
		else if (std::strcmp(args.c_str(), kNNDistanceWeightToStr[kNN::ONE_MINUS_DISTANCE]) == 0)
			mDistWeight = kNN::ONE_MINUS_DISTANCE;
	}
};

//implement interface
MyoClassifierkNN::~MyoClassifierkNN()
{
}


MyoModelInterface* MyoClassifierkNN::createModel(const DataSetReader& ds)
{
    mModel = std::make_shared<MyoModelkNN>(ds);
    return mModel.get();
}

MyoModelInterface* MyoClassifierkNN::createModel(const DataSetReader& ds,const std::string& args)
{
    mModel = std::make_shared<MyoModelkNN>(ds,args);
    return mModel.get();
}

MyoModelInterface* MyoClassifierkNN::modelFromfile(const std::string& path)
{
    mModel = std::make_shared<MyoModelkNN>();
    mModel->deserialize(path);
    return mModel.get();
}

void MyoClassifierkNN::setModel(MyoModelInterface* model)
{
	mModel = std::shared_ptr<MyoModelkNN>((MyoModelkNN*)model);
}

MyoModelInterface* MyoClassifierkNN::getModel() const
{
	return mModel.get();
}

void MyoClassifierkNN::setProbabilityFilter(double probability)
{
    mProbabilityFilter = probability;
}

void MyoClassifierkNN::classification(MyoThread& myo,
                                      const std::function< void (const std::string& className) > callback)
{
    myo.start([this,callback](const MyoThread::Inputs& inputs,
                              const DataFlags& flags,
                              std::mutex& mutex)
              {
                  if(!inputs.size()) return;
                  //loop constants
                  size_t rowSize = flags.lineSize<8>() / flags.mReps;
                  size_t allSize = rowSize*inputs.size();
                  //alloc row
				  DataSetReader::Row dataRow(allSize);
                  //put values
                  MyoThread::applay(inputs,
                                    flags,
                                    [&](size_t i,size_t ri, double value)
                                    {
										dataRow[i] = value;
                                    });
                  //do classification
                  kNN::Result res = mModel->classify(dataRow);
                  
                  //filters
                  if(mProbabilityFilter <= (1.0-res.mError))
                  if(mLastClass != res.mClasses)
                  {
                      //callback
                      callback(mModel->mData.getClassNames().getClassName(res.mClasses));
                      mLastClass = res.mClasses;
                  }
              },
              mModel->mData.getFlags(),
              mModel->mData.getUpdate());
}


