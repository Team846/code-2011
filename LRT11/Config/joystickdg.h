// joystick control mappings
// D.Giandomenico

#ifndef joytick_h_
#define joystick_h_




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
//      |
//  0 --+-- 16383     RZ 0-255 clockwise
//      |
//    16383
//
//      0
//      |   (Slider)
//     255


#endif joystick_h_