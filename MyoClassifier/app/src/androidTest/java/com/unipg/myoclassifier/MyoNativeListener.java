package com.unipg.myoclassifier;


import com.thalmic.myo.DeviceListener;
import com.thalmic.myo.Arm;
import com.thalmic.myo.Myo;
import com.thalmic.myo.Pose;
import com.thalmic.myo.Quaternion;
import com.thalmic.myo.Vector3;
import com.thalmic.myo.XDirection;

/**
 * Created by Gabriele on 12/10/15.
 */
public class MyoNativeListener implements DeviceListener {

    private native void onAttach(long l);
    private native void onDetach(long l);
    private native void onConnect(long l);
    private native void onDisconnect(long l);
    private native void onArmSync(long l);
    private native void onArmUnsync(long l);
    private native void onUnlock(long l);
    private native void onLock(long l);
    private native void onPose(long l, int pose);
    private native void onQuaternion(long l, double x, double y, double z, double w);
    private native void onAccelerometer(long l, double x, double y, double z);
    private native void onGyroscope(long l, double x, double y, double z);


    @Override
    public void onAttach(Myo myo, long l) {
        this.onAttach(l);
    }

    @Override
    public void onDetach(Myo myo, long l) {
        this.onDetach(l);
    }

    @Override
    public void onConnect(Myo myo, long l) {
        this.onConnect(l);
    }

    @Override
    public void onDisconnect(Myo myo, long l) {
        this.onDisconnect(l);
    }

    @Override
    public void onArmSync(Myo myo, long l, Arm arm, XDirection xDirection) {
        this.onArmSync(l);
    }

    @Override
    public void onArmUnsync(Myo myo, long l) {
        this.onArmUnsync(l);
    }

    @Override
    public void onUnlock(Myo myo, long l) {
        this.onUnlock(l);
    }

    @Override
    public void onLock(Myo myo, long l) {
        this.onLock(l);
    }

    @Override
    public void onPose(Myo myo, long l, Pose pose) {
        switch (pose)
        {
            case REST:             this.onPose(l, 0); break;
            case FIST:             this.onPose(l, 1); break;
            case WAVE_IN:          this.onPose(l, 2); break;
            case WAVE_OUT:         this.onPose(l, 3); break;
            case FINGERS_SPREAD:   this.onPose(l, 4); break;
            case DOUBLE_TAP:       this.onPose(l, 5); break;
            case UNKNOWN: default: this.onPose(l, 6); break;
        }
    }

    @Override
    public void onOrientationData(Myo myo, long l, Quaternion quaternion) {
        this.onQuaternion(l,quaternion.x(),quaternion.y(),quaternion.z(),quaternion.w());
    }

    @Override
    public void onAccelerometerData(Myo myo, long l, Vector3 vector3) {
        this.onAccelerometer(l, vector3.x(), vector3.y(), vector3.z());
    }

    @Override
    public void onGyroscopeData(Myo myo, long l, Vector3 vector3) {
        this.onGyroscope(l, vector3.x(), vector3.y(), vector3.z());
    }

    @Override
    public void onRssi(Myo myo, long l, int i) {

    }
}
