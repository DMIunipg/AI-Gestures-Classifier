//
//  main.cpp
//  FANNMyo
//
//  Created by Gabriele Di Bari on 02/07/15.
//  Copyright (c) 2015 Gabriele Di Bari. All rights reserved.
//

#include <iostream>
#include <fann/floatfann.h>
#include <myo.hpp>
#include <array>
#include <time.h>
#include "Utilities.h"

#define MAP_ANGLE( x ) x
#define MAP_EMG( x ) x

class DataCollector : public myo::DeviceListener
{
public:
    
    DataCollector(){}
    
    void onUnpair(myo::Myo* myo, uint64_t timestamp)
    {
        mArm.clear();
        mCurrentPose = myo::Pose();
        mRaw.clear();
    }
    
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
    {
        mRaw.setQuaternion(quat);
    }
    
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
    {
        //save
        mCurrentPose = pose;
#if 0
        //notify
        if (pose != myo::Pose::unknown && pose != myo::Pose::rest)
        {
            myo->unlock(myo::Myo::unlockHold);
            myo->notifyUserAction();
        }
        else
        {
            myo->unlock(myo::Myo::unlockTimed);
        }
#endif
    }
    
    
    void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
                   myo::WarmupState warmupState)
    {
        mArm.setArm(arm);
        mArm.sync();
    }
    
    void onArmUnsync(myo::Myo* myo, uint64_t timestamp)
    {
        mArm.unsync();
    }
    
    void onUnlock(myo::Myo* myo, uint64_t timestamp)
    {
        mArm.unlock();
    }
    
    void onLock(myo::Myo* myo, uint64_t timestamp)
    {
        mArm.lock();
    }
    
    //raw values
    void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel)
    {
        mRaw.setAccelerometer(accel);
    }
    void onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro)
    {
        mRaw.setGyroscope(gyro);
    }
    void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg)
    {
        mRaw.setEmgCArray(emg, 8);
    }
    
    
    std::string toString()
    {
        std::string out;
        out += mArm.toString()    + "\n";
        out += mRaw.toString()    ;//+ "\n";
        out += "pos: " + mCurrentPose.toString();
        return out;
    }
    
    myo::ArmStatus mArm;
    myo::Pose mCurrentPose;
    myo::RawDatas<int8_t, float, float, float> mRaw;
};

int main(int argc, const char * argv[])
{
#if 1
    const unsigned int num_layers = 3;
    const float desired_error = (const float) 0.005;
    
    //train data
    struct fann_train_data *train_data= fann_read_train_from_file("datas/data_.fann");
    //
    float momentum = 2.0/train_data->num_input;
    //ANN
    struct fann* ann = fann_create_standard(num_layers,
                                            train_data->num_input,
                                            train_data->num_input*3,
                                            train_data->num_output);
    
    std::cout << "Training network.\n";
    
    fann_set_training_algorithm(ann, FANN_TRAIN_INCREMENTAL);
    //fann_set_learning_rate(ann, 0.95f);
    fann_set_learning_momentum(ann,momentum);
    //fann_set_train_error_function(ann, FANN_ERRORFUNC_LINEAR);
    
    fann_train_on_data(ann, train_data, 3000, 10, desired_error);
    
    std::cout << "Testing network.\n";
    
    struct fann_train_data* test_data = fann_read_train_from_file("datas/test_.fann");
    
    fann_reset_MSE(ann);
    for(int i = 0; i < fann_length_train_data(test_data); i++)
    {
        fann_test(ann, test_data->input[i], test_data->output[i]);
    }
    
    std::cout << "MSE error on test data: " << fann_get_MSE(ann) << "\n";
    
    std::cout << "Saving network.\n";
    
    fann_save(ann, "data_model.net");
    
    std::cout << "Cleaning up.\n";
    
    fann_destroy_train(train_data);
    fann_destroy_train(test_data);
#else
    struct fann *ann = fann_create_from_file("data_model.net");
#endif
    
    size_t row_size=0;
    MAP_ANGLE(row_size += 3);
    MAP_EMG  (row_size += 8);
    
    
    fann_type *calc_out;
    fann_type input[(row_size)*3];

    try
    {
        myo::Hub hub("com.runtime.fnnamyo");
        myo::Myo* myo = hub.waitForMyo(10000);
        myo->setStreamEmg(myo::Myo::streamEmgEnabled);
        DataCollector collector;
        hub.addListener(&collector);
        //loop
        bool loop = true;
        //count
        int count = 0;
        //
        while (loop)
        {
            hub.run(1000/20);
            if(collector.mArm.isUnlock())
            {
                //get angles
                auto  angles =collector.mRaw.getEulerAngles();
                auto& emg    =collector.mRaw.getEmg();
                //set input data
                MAP_ANGLE
                (
                    input[0+row_size*count] = (double)angles.roll() / M_PI_2;
                    input[1+row_size*count] = (double)angles.pitch() / M_PI_2;
                    input[2+row_size*count] = (double)angles.yaw() / M_PI_2;
                )
                MAP_EMG
                (
                     for(int i=0;i!=8;++i)
                        input[i+3+row_size*count] = (double)emg[i] / 128;
                )
                //update count
                if(count == 2)
                {
                    //run
                    calc_out = fann_run(ann,input);
                    //print
                    std::cout << "-----------------------------------------------\n";
                    std::cout << "arm in in status: "<< (float)(calc_out[0]) << "\n";
                    std::cout << "-----------------------------------------------\n";
                    //restart
                    count = 0;
                }
                else
                {
                    //std::cout << collector.toString() <<"\n";
                    ++count;
                }
            }
            else
            {
                std::cout << "myo not work..\n";
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Press enter to continue.";
        std::cin.ignore();
        return 1;
    }
    //free
    fann_destroy(ann);
    //
    return 0;
}
