// joystick control mappings
// D.Giandomenico
//#include "Joystick.h"
#include "DriverStation.h"
#include <queue>

#ifndef joystickdg_h_
#define joystickdg_h_

#include "../Util/Util.h"
#warning ProcessedInputs is compiling


class driverstation_button
{
protected:
    int const USBPort_;
    int const button_no_;
    int keymap_index_; //index of the key into the raw bits from the driverstation
    const char* const name_;
public:
    int Keymap_Index()
    {
        return keymap_index_;
    }
    driverstation_button(int USBPort, int button_no, char* name)
        : USBPort_(USBPort)
        , button_no_(button_no)
        , name_(name ? name : "")
    {
        //keymap and other data that may require
        //initialization is done in Validate_ButtonOrDie()
        //so we may debug when main code is running
        //and force user to Validate by not initializing here.

        //insert into list of buttons.
        next_button_ = button_list_;
        button_list_ = this;
    }
    ~driverstation_button() {}

    static int Initalize_All_Buttons(); //returns zero status if no error.
    //use as Initalize() || Die().

protected:
    int Validate_Button();
    driverstation_button* next_button_;
    static driverstation_button* button_list_;
};



namespace DriverStationButtons
{
    driverstation_button example(0, 3, "example");
    driverstation_button trigger(1, 1, "trigger");
    driverstation_button     hat(1, 2, "hat");
    driverstation_button    lift(1, 3, "lift");
}

//How can we have different button mappings depending on the mode?
//Would like a base mapping, and then special assignments.
//We don't want butons defined in a class because they can't be initialized at the time they are delcard.
//Can we use namespaces?
//Should we add a mode to the key assignment, e,g, disabled|auto|service|teleop?
//Could pass a static driverstation to create maps, or create maps based on above flags.

namespace a1
{
    const int funny = 10;
}
namespace a2
{

}
int aa = a1::funny;
const int eee = 1;
enum { ddd = 1};
#if (static_cast <char>(0xEEFF)== static_cast <char> (0x0102))
#warning true
#else
#warning false
#endif
#warning
driverstation_button::driverstation_button* driverstation_button::button_list_ = NULL;
/*
 * Validates assigned values for all buttons.
 * Do this after creation of the buttons in the main code
 * since buttons may be static variables.
 * Calculates the keymap_index into the raw driver station data.
 */
int
driverstation_button::Initalize_All_Buttons()
{
    bool all_buttons_valid = true;
    driverstation_button* b;
    for(b = driverstation_button::button_list_; b != NULL; b = b->next_button_)
    {
        int error_status = b->Validate_Button();
        if(0 != error_status)
        {
            all_buttons_valid = false;
            continue;
        }
        //complete initialization
        b->keymap_index_ = (b->USBPort_ - 1) << 4 | (b->button_no_ - 1);
    }
    return all_buttons_valid ? 0 : 1;   //return status=0 on no errors.
}

/*
 * Validate_Button()
 * Ensure each button assignment is within range.
 * Do this check at initialization.
 */
int
driverstation_button::Validate_Button()
{
    //range of buttons is 1-12 (see WPLib joystick.h/.c etc.) -dg
    if(USBPort_ > 0 && USBPort_ < 4 && button_no_ > 0 && button_no_ <= 12)
    {
        printf("Key assignment out of range. USBPort=%d, Button=%d. Name=%.20s ",
                USBPort_, button_no_, name_);
        return 1;
    }
    return 0;
}

driverstation_button key1(1, 2, "funny");


//Joystick buttons -- Thrustmaster T.16000m
enum joystick
{
    kTrigger = 1, kThumb = 2, kThumbLeft = 3, kThumbRight = 4,
    kButton5, kButton6, kButton7,   //  5  6  7  Left side
    kButton8, kButton9, kButton10,  // 10  9  8  button layout

    kButton11, kButton12, kButton13, // 13 12 11  Right side
    kButton14, kButton15, kButton16, // 14 15 16  button layout
};

//      0
//      |s
//  0 --+-- 16383     RZ 0-255 clockwise
//      |
//    16383
//
//      0
//      |   (Slider)
//     255

namespace DRIVER_INPUT
{
    const int joy1 = 0x0000;
    const int joy2 = 0x0100;
    const int kButton1 = joy1 | 00;
}
typedef queue<int>  INTQUEUE;
typedef queue<driverstation_button>  EventQueue;

EventQueue theQueue;

class DriverInputs
{
public:
    DriverInputs();
    ~DriverInputs();

    bool ButtonDown(driverstation_button& button);
    bool ButtonPressed(driverstation_button& button);
    bool ButtonReleased(driverstation_button& button);

    void Update();
private:
    DriverStation* ds_;
    struct driverstationRawData
    {
        UINT64 down_;
        UINT64 changed_;
    } raw_buttons_;
    int DriverInputs::ButtonIndexMap(UINT16 button_id);

};


void DriverInputs::Update()
{
    //save the buttons so we can see which changed later.
    UINT64 old_buttons = raw_buttons_.down_;

    UINT64 temp = 0;

    temp  = static_cast<UINT16>(ds_->GetStickButtons(3));
    temp <<= 16;
    temp |= static_cast<UINT16>(ds_->GetStickButtons(2));
    temp <<= 16;
    temp |= static_cast<UINT16>(ds_->GetStickButtons(1));
    temp <<= 16;
    temp |= static_cast<UINT16>(ds_->GetStickButtons(0));

    raw_buttons_.down_ = temp;
    raw_buttons_.changed_ = raw_buttons_.down_ ^ old_buttons;
    //should add these to the event queue. -dg TODO

    //empty queue
    while(!theQueue.empty())
        theQueue.pop();
    theQueue.push(DriverStationButtons::example);
    theQueue.front();
    theQueue.back();
    theQueue.size();

    UINT64 raw_button_down_events = raw_buttons_.changed_ ^ raw_buttons_.down_;
    for(int i = 0;  0 != raw_button_down_events; (++i, raw_button_down_events >>= 1)) //a button has been pressed or released.
    {
        if(0x1 & raw_button_down_events)
            theQueue.push(DriverStationButtons::example); //TODO: need to add the actual event
    }
}
//x& x - 1
//
//0111000100
//0111000011

//Returns true if button is down
bool DriverInputs::ButtonDown(driverstation_button& button)
{
    int key_index = button.Keymap_Index();
    return 0x1 & (raw_buttons_.down_ >> key_index);
}

//Returns true for one cycle when a button is pressed.
bool DriverInputs::ButtonPressed(driverstation_button& button)
{
    int key_index = button.Keymap_Index();
    return 0x1 & ((raw_buttons_.changed_ & raw_buttons_.down_) >> key_index);
}

//Returns true for one cycle when button is released.
bool DriverInputs::ButtonReleased(driverstation_button& button)
{
    int key_index = button.Keymap_Index();
    return 0x1 & ((raw_buttons_.changed_ & ~raw_buttons_.down_) >> key_index);
}
#endif //joystickdg_h_
