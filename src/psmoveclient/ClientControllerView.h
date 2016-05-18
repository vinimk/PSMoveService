#ifndef CLIENT_CONTROLLER_VIEW_H
#define CLIENT_CONTROLLER_VIEW_H

//-- includes -----
#include "ClientConfig.h"
#include "ClientConstants.h"
#include "ClientGeometry.h"
#include <cassert>

//-- pre-declarations -----
namespace PSMoveProtocol
{
    class DeviceDataFrame;
    class DeviceDataFrame_ControllerDataPacket;
};

//-- constants -----
enum PSMoveButtonState {
    PSMoveButton_UP = 0x00,       // (00b) Not pressed
    PSMoveButton_PRESSED = 0x01,  // (01b) Down for one frame only
    PSMoveButton_DOWN = 0x03,     // (11b) Down for >1 frame
    PSMoveButton_RELEASED = 0x02, // (10b) Up for one frame only
};

//-- declarations -----
struct CLIENTPSMOVEAPI PSMoveRawSensorData
{
    PSMoveIntVector3 Magnetometer;
    PSMoveFloatVector3 Accelerometer;
    PSMoveFloatVector3 Gyroscope;

    inline void Clear()
    {
        Magnetometer= *k_psmove_int_vector3_zero;
        Accelerometer= *k_psmove_float_vector3_zero;
        Gyroscope= *k_psmove_float_vector3_zero;
    }
};

struct CLIENTPSMOVEAPI PSMoveRawTrackerData
{
    // Parallel arrays: ScreenLocation and the TrackerID associated with it
    PSMoveScreenLocation ScreenLocations[PSMOVESERVICE_MAX_TRACKER_COUNT];
    int TrackerIDs[PSMOVESERVICE_MAX_TRACKER_COUNT];
    int ValidTrackerLocations;

    inline void Clear()
    {
        for (int index = 0; index < PSMOVESERVICE_MAX_TRACKER_COUNT; ++index)
        {
            ScreenLocations[index] = PSMoveScreenLocation::create(0, 0);
            TrackerIDs[index] = -1;
        }
        ValidTrackerLocations = 0;
    }

    inline bool GetLocationForTrackerId(int trackerId, PSMoveScreenLocation &outLocation) const
    {
        bool bFound = false;

        for (int listIndex = 0; listIndex < ValidTrackerLocations; ++listIndex)
        {
            if (TrackerIDs[listIndex] == trackerId)
            {
                outLocation = ScreenLocations[listIndex];
                bFound = true;
                break;
            }
        }

        return bFound;
    }
};

struct CLIENTPSMOVEAPI ClientPSMoveView
{
private:
    bool bValid;
    bool bHasValidHardwareCalibration;
    bool bIsTrackingEnabled;
    bool bIsCurrentlyTracking;

    PSMovePose Pose;
    PSMoveRawSensorData RawSensorData;
    PSMoveRawTrackerData RawTrackerData;

    PSMoveButtonState TriangleButton;
    PSMoveButtonState CircleButton;
    PSMoveButtonState CrossButton;
    PSMoveButtonState SquareButton;
    PSMoveButtonState SelectButton;
    PSMoveButtonState StartButton;
    PSMoveButtonState PSButton;
    PSMoveButtonState MoveButton;
    PSMoveButtonState TriggerButton;

    unsigned char TriggerValue;

    unsigned char CurrentRumble;
    unsigned char RumbleRequest;

public:
    void Clear();
    void ApplyControllerDataFrame(const PSMoveProtocol::DeviceDataFrame_ControllerDataPacket *data_frame);

    inline bool IsValid() const
    {
        return bValid;
    }

    inline void SetValid(bool flag)
    {
        bValid= flag;
    }

    inline bool GetHasValidHardwareCalibration() const
    {
        return IsValid() ? bHasValidHardwareCalibration : false;
    }

    inline bool GetIsCurrentlyTracking() const
    {
        return IsValid() ? bIsCurrentlyTracking : false;
    }
    
    inline bool GetIsTrackingEnabled() const
    {
        return IsValid() ? bIsTrackingEnabled : false;
    }

    inline const PSMovePosition &GetPosition() const
    {
        return IsValid() ? Pose.Position : *k_psmove_position_origin;
    }

    inline const PSMoveQuaternion &GetOrientation() const
    {
        return IsValid() ? Pose.Orientation : *k_psmove_quaternion_identity;
    }

    inline PSMoveButtonState GetButtonTriangle() const
    {
        return IsValid() ? TriangleButton : PSMoveButton_UP;
    }
    
    inline PSMoveButtonState GetButtonCircle() const
    {
        return IsValid() ? CircleButton : PSMoveButton_UP;
    }
    
    inline PSMoveButtonState GetButtonCross() const
    {
        return IsValid() ? CrossButton : PSMoveButton_UP;
    }
    
    inline PSMoveButtonState GetButtonSquare() const
    {
        return IsValid() ? SquareButton : PSMoveButton_UP;
    }
    
    inline PSMoveButtonState GetButtonSelect() const
    {
        return IsValid() ? SelectButton : PSMoveButton_UP;
    }
    
    inline PSMoveButtonState GetButtonStart() const
    {
        return IsValid() ? StartButton : PSMoveButton_UP;
    }
    
    inline PSMoveButtonState GetButtonPS() const
    {
        return IsValid() ? PSButton : PSMoveButton_UP;
    }
    
    inline PSMoveButtonState GetButtonMove() const
    {
        return IsValid() ? MoveButton : PSMoveButton_UP;
    }

    inline PSMoveButtonState GetButtonTrigger() const
    {
        return IsValid() ? TriggerButton : PSMoveButton_UP;
    }

    inline float GetTriggerValue() const
    {
        return IsValid() ? ((float)TriggerValue / 255.f) : 0.f;
    }

    const PSMoveRawSensorData &GetRawSensorData() const;
    const PSMoveFloatVector3 &GetIdentityGravityCalibrationDirection() const;
    bool GetIsStableAndAlignedWithGravity() const;

    const PSMoveRawTrackerData &GetRawTrackerData() const;
};

class CLIENTPSMOVEAPI ClientPSNaviView
{
private:
    bool bValid;

    PSMoveButtonState L1Button;
    PSMoveButtonState L2Button;
    PSMoveButtonState L3Button;
    PSMoveButtonState CircleButton;
    PSMoveButtonState CrossButton;
    PSMoveButtonState PSButton;
    PSMoveButtonState TriggerButton;
    PSMoveButtonState DPadUpButton;
    PSMoveButtonState DPadRightButton;
    PSMoveButtonState DPadDownButton;
    PSMoveButtonState DPadLeftButton;

    unsigned char TriggerValue;
    unsigned char Stick_XAxis;
    unsigned char Stick_YAxis;

public:
    void Clear();
    void ApplyControllerDataFrame(const PSMoveProtocol::DeviceDataFrame_ControllerDataPacket *data_frame);

    inline bool IsValid() const
    {
        return bValid;
    }

    inline PSMoveButtonState GetButtonL1() const
    {
        return IsValid() ? L1Button : PSMoveButton_UP;
    }

    inline PSMoveButtonState GetButtonL2() const
    {
        return IsValid() ? L2Button : PSMoveButton_UP;
    }

    inline PSMoveButtonState GetButtonL3() const
    {
        return IsValid() ? L3Button : PSMoveButton_UP;
    }

    inline PSMoveButtonState GetButtonCircle() const
    {
        return IsValid() ? CircleButton : PSMoveButton_UP;
    }
    
    inline PSMoveButtonState GetButtonCross() const
    {
        return IsValid() ? CrossButton : PSMoveButton_UP;
    }

    inline PSMoveButtonState GetButtonPS() const
    {
        return IsValid() ? PSButton : PSMoveButton_UP;
    }

    inline PSMoveButtonState GetButtonTrigger() const
    {
        return IsValid() ? TriggerButton : PSMoveButton_UP;
    }

    inline PSMoveButtonState GetButtonDPadUp() const
    {
        return IsValid() ? DPadUpButton : PSMoveButton_UP;
    }

    inline PSMoveButtonState GetButtonDPadRight() const
    {
        return IsValid() ? DPadRightButton : PSMoveButton_UP;
    }

    inline PSMoveButtonState GetButtonDPadDown() const
    {
        return IsValid() ? DPadDownButton : PSMoveButton_UP;
    }

    inline PSMoveButtonState GetButtonDPadLeft() const
    {
        return IsValid() ? DPadLeftButton : PSMoveButton_UP;
    }

    inline float GetTriggerValue() const
    {
        return IsValid() ? ((float)TriggerValue / 255.f) : 0.f;
    }

    inline float GetStickXAxis() const
    {
        float float_value= 0.f;

        if (IsValid())
        {
            int signed_value= static_cast<int>(Stick_XAxis) - 0x80;

            float_value= static_cast<float>(signed_value) / static_cast<float>(0x80);
        }

        return float_value;
    }

    inline float GetStickYAxis() const
    {
        float float_value= 0.f;

        if (IsValid())
        {
            int signed_value= static_cast<int>(Stick_YAxis) - 0x80;

            float_value= static_cast<float>(signed_value) / static_cast<float>(0x80);
        }

        return float_value;
    }
};

class CLIENTPSMOVEAPI ClientControllerView
{
public:
    enum eControllerType
    {
        None= -1,
        PSMove,
        PSNavi
    };

private:
    union
    {
        ClientPSMoveView PSMoveView;
        ClientPSNaviView PSNaviView;
    } ViewState;
    eControllerType ControllerViewType;

    int ControllerID;
    int SequenceNum;
    int ListenerCount;

    bool IsConnected;

    long long data_frame_last_received_time;
    float data_frame_average_fps;

public:
    ClientControllerView(int ControllerID);

    void Clear();
    void ApplyControllerDataFrame(const PSMoveProtocol::DeviceDataFrame_ControllerDataPacket *data_frame);

    // Listener State
    inline void IncListenerCount()
    {
        ++ListenerCount;
    }

    inline void DecListenerCount()
    {
        assert(ListenerCount > 0);
        --ListenerCount;
    }

    inline int GetListenerCount() const
    {
        return ListenerCount;
    }

    // Controller Data Accessors
    inline int GetControllerID() const
    {
        return ControllerID;
    }

    inline int GetSequenceNum() const
    {
        return IsValid() ? SequenceNum : -1;
    }

    inline eControllerType GetControllerViewType() const
    {
        return ControllerViewType;
    }

    inline const ClientPSMoveView &GetPSMoveView() const
    {
        assert(ControllerViewType == PSMove);
        return ViewState.PSMoveView;
    }

    inline ClientPSMoveView &GetPSMoveViewMutable()
    {
        assert(ControllerViewType == PSMove);
        return ViewState.PSMoveView;
    }

    inline const ClientPSNaviView &GetPSNaviView() const
    {
        assert(ControllerViewType == PSNavi);
        return ViewState.PSNaviView;
    }

    inline bool IsValid() const
    {
        return ControllerID != -1;
    }

    inline bool GetIsConnected() const
    {
        return (IsValid() && IsConnected);
    }
    
    // Statistics
    inline float GetDataFrameFPS() const
    {
        return data_frame_average_fps;
    }
};
#endif // CLIENT_CONTROLLER_VIEW_H