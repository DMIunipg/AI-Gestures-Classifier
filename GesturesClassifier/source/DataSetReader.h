//
//  DataSetReader.hpp
//  GesturesClassifier
//
//  Created by Gabriele Di Bari on 10/10/15.
//  Copyright © 2015 Gabriele Di Bari. All rights reserved.
//

#ifndef DataSetReader_h
#define DataSetReader_h

#include <stdio.h>
#include <string>
#include <functional>
#include <vector>
#include <cmath>
#include "DataFlags.h"

/*!
 * \brief DataSetReader
 */
class DataSetReader
{
public:
    
    /*!
     * \brief type of a list of raw data
     */
    using AttributesRaw = std::vector< double >;

    /*!
     * \brief Row the type of a dataset row
     */
    struct Row
    {
        double                mClass { 0.0 };  //! class name
        AttributesRaw         mRaw;            //! raw data
        
		Row(){}
		Row(size_t size) :mRaw(size) {}
		Row(double rowClass,size_t size) :mClass(rowClass),mRaw(size) {}

        /*!
         * \brief size
         * \return the count of raw data
         */
        size_t size() const{ return mRaw.size(); }
        
        /*!
         * \brief operator[]
         * \param i
         * \return the raw data at index i
         */
        double& operator[] (size_t i)      { return mRaw[i]; }
        
        /*!
         * \brief operator[]
         * \param i
         * \return the raw data at index i
         */
        double  operator[] (size_t i) const{ return mRaw[i]; }


		/*!
		* \brief getEmgFromRow
		* \param row
		* \param flags
		* \return emg values of a row
		*/
		template< const size_t N_EMG = 8, typename OType >
		static std::array< std::vector< OType >, N_EMG > getEmgFromRow(const Row& iRow,const DataFlags& dataFlags)
		{
			using EmgRow = std::vector< OType >;
			//global size
			size_t sampleSize = dataFlags.sampleLineSize<N_EMG>();
			//local position
			size_t emgSubPos = dataFlags.getEmgSampleEmgPosition();
			//alloc
			std::array< EmgRow, N_EMG > emgs_N;
			//copy
			for (size_t rep = 0; rep != dataFlags.mReps; ++rep)
			{
				//get all emgs
				for (size_t emg = 0; emg != N_EMG; ++emg)
				{
					emgs_N[emg].push_back(OType(iRow[rep*sampleSize + emgSubPos + emg]));
				}
			}
			//return
			return emgs_N;
		}

		/*!
		* \brief setEmgToRow
		* \param row
		* \param flags
		* \param emg values
		*/
		template< const size_t N_EMG = 8, typename IType >
		static bool setEmgToRow(Row& iRow, const DataFlags& dataFlags, const std::array< std::vector<IType>, N_EMG >& emgs_N)
		{
			using EmgRow = std::vector<IType>;
			//global size
			size_t sampleSize = dataFlags.sampleLineSize<N_EMG>();
			//local position
			size_t emgSubPos = dataFlags.getEmgSampleEmgPosition();
			//test size
			if (emgs_N[0].size() != dataFlags.mReps) return false;
			//copy
			for (size_t rep = 0; rep != dataFlags.mReps; ++rep)
			{
				//set all emgs
				for (size_t emg = 0; emg != N_EMG; ++emg)
				{
					iRow[rep*sampleSize + emgSubPos + emg] = (double)emgs_N[emg][rep];
				}
			}
			return true;
		}

		/*!
		* \brief setAbsEmgToRow
		* \param row
		* \param flags
		* \param emg values
		*/
		template< const size_t N_EMG = 8, typename IType >
		static bool setAbsEmgToRow(Row& iRow, const DataFlags& dataFlags, const std::array< std::vector<IType>, N_EMG >& emgs_N)
		{
			using EmgRow = std::vector<IType>;
			//global size
			size_t sampleSize = dataFlags.sampleLineSize<N_EMG>();
			//local position
			size_t emgSubPos = dataFlags.getEmgSampleEmgPosition();
			//test size
			if (emgs_N[0].size() != dataFlags.mReps) return false;
			//copy
			for (size_t rep = 0; rep != dataFlags.mReps; ++rep)
			{
				//set all emgs
				for (size_t emg = 0; emg != N_EMG; ++emg)
				{
					iRow[rep*sampleSize + emgSubPos + emg] = (double)std::abs(emgs_N[emg][rep]);
				}
			}
			return true;
		}
    };
    
    /*!
     * \brief Rows, list of dataset row
     */
    using Rows    =  std::vector< Row >;
    
    /*!
     * \brief Rows iterator
     */
    using RowsIt  =  Rows::iterator;
    
    /*!
     * \brief Rows constant iterator
     */
    using RowsCIt =  Rows::const_iterator;
    
    /*!
     *  \brief read from file
     *  \param path
     *  \return true if success to read
     */
    bool read(const std::string& path);
    
    /*!
     * \brief serialize into a file
     */
    void serialize(FILE* file);
    
    /*!
     * \brief deserialize from a file
     */
    void deserialize(FILE* file);
    
    /*!
     * \brief getFlags, return metadata
     * \return metadata
     */
    const DataFlags&    getFlags()      const { return mDataFlags; }
    
    /*!
     * \brief getClassNames, return list of classes
     */
    const ClassesNames& getClassNames() const { return mClassNames; }
    
    /*!
     * \brief return update value
     */
    unsigned int        getUpdate() const { return mUpdate; }
    
    /*!
     * \brief return dataset rows
     */
    const Rows&         getRows() const       { return mRows; }
    
    /*!
     * \brief size 
     * \return number of rows
     */
    size_t size() const{ return mRows.size(); }
    
    /*!
     * \brief sizeLine
     * \return size of a row
     */
    size_t sizeLine() const{ return mRows.size() ?  mRows[0].size() : 0; }
    
    /*!
     * \brief operator[]
     * \param i
     * \return the row at index i
     */
    Row& operator[] (size_t i)       { return mRows[i]; }
    
    /*!
     * \brief operator[]
     * \param i
     * \return the row at index i
     */
    const Row& operator[] (size_t i) const { return mRows[i]; }
    
    /*!
     * \brief begin
     * \return the first iterator object
     */
    RowsIt begin()        { return mRows.begin(); }
    
    /*!
     * \brief end
     * \return the last iterator object
     */
    RowsIt end()          { return mRows.end();   }
    
    /*!
     * \brief begin
     * \return the first constant iterator object
     */
    RowsCIt begin() const { return mRows.begin(); }
    
    
    /*!
     * \brief end
     * \return the last constant iterator object
     */
    RowsCIt end() const   { return mRows.end();   }

	/*!
	* \brief getEmgFromRow
	* \param row index
	* \return emg values of a row
	*/
	template< const size_t N_EMG = 8, typename OType >
	std::array< std::vector< OType >, N_EMG > getEmgFromRow(size_t index) const
	{
		//return
		return Row::getEmgFromRow<N_EMG,OType>(mRows[index], mDataFlags);
	}

	/*!
	* \brief setEmgToRow
	* \param row index
	* \param emg values
	*/
	template< const size_t N_EMG = 8, typename IType >
	bool setEmgToRow(size_t index, const std::array< std::vector<IType>, N_EMG >& emgs_N)
	{
		return true;
		//return
		return Row::setEmgToRow<N_EMG, IType>(mRows[index], mDataFlags, emgs_N);
	}

	/*!
	* \brief setEmgToRow
	* \param row index
	* \param emg values
	*/
	template< const size_t N_EMG = 8, typename IType >
	bool setAbsEmgToRow(size_t index, const std::array< std::vector<IType>, N_EMG >& emgs_N)
	{
		return Row::setAbsEmgToRow<N_EMG, IType>(mRows[index], mDataFlags, emgs_N);
	}


protected:
    
    //meta data
    DataFlags    mDataFlags;
    ClassesNames mClassNames;
    unsigned int mUpdate{ 0 };
    //Rows
    Rows mRows;
    
    
};

#endif /* DataSetReader_h */
