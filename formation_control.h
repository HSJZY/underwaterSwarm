#ifndef FORMATION_CONTROL_H
#define FORMATION_CONTROL_H
#include"camera.h"

class formation_control
{
public:
    formation_control();
    void line_formation_control(float direction_angle,float inter_distance);
private:
    Mat grab_pictures();
};

#endif // FORMATION_CONTROL_H
