package com.unipg.myoclassifierparrot;


import eu.darken.myolib.processor.imu.ImuData;
import eu.darken.myolib.processor.imu.ImuProcessor.ImuDataListener;
import eu.darken.myolib.processor.emg.EmgData;
import eu.darken.myolib.processor.emg.EmgProcessor.EmgDataListener;

/**
 * Created by Gabriele on 12/10/15.
 */
public class MyoNativeListener implements EmgDataListener, ImuDataListener {

    /**
     * @brief onEmg, method to send emgs values to native listener
     * @param emg0
     * @param emg1
     * @param emg2
     * @param emg3
     * @param emg4
     * @param emg5
     * @param emg6
     * @param emg7 
     */
    public native void onEmg(int emg0,
                             int emg1,
                             int emg2,
                             int emg3,
                             int emg4,
                             int emg5,
                             int emg6,
                             int emg7);
    /**
     * @brief onQuaternion, send quaternion to native listener
     * @param x 
     * @param y 
     * @param z 
     * @param w 
     */
    public native void onQuaternion(double x, double y, double z, double w);

    /**
     * @brief onAccelerometer, send accelerometer values to native listener
     * @param x 
     * @param y 
     * @param z 
     */
    public native void onAccelerometer(double x, double y, double z);

    /**
     * @brief onGyroscope, send gyroscope values to native listener
     * @param x 
     * @param y 
     * @param z 
     */
    public native void onGyroscope(double x, double y, double z);

    /**
     * @brief onNewEmgData 
     * @param emgData
     */
    @Override
    public void onNewEmgData(EmgData emgData) {
        this.onEmg( emgData.getData()[0],
                    emgData.getData()[1],
                    emgData.getData()[2],
                    emgData.getData()[3],
                    emgData.getData()[4],
                    emgData.getData()[5],
                    emgData.getData()[6],
                    emgData.getData()[7]);

    }

    /**
     * @brief onNewImuData
     * @param imuData 
     */
    @Override
    public void onNewImuData(ImuData imuData) {
        this.onAccelerometer(imuData.getAccelerometerData()[0],
                             imuData.getAccelerometerData()[1],
                             imuData.getAccelerometerData()[2]);

        this.onGyroscope(imuData.getGyroData()[0],
                         imuData.getGyroData()[1],
                         imuData.getGyroData()[2]);

        this.onQuaternion(imuData.getOrientationData()[0],
                          imuData.getOrientationData()[1],
                          imuData.getOrientationData()[2],
                          imuData.getOrientationData()[3]);
    }
}
