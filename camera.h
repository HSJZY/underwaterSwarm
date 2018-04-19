#ifndef CAMERA_H
#define CAMERA_H


class camera
{
public:
    camera();
    void OpenCamera();
    void CapturePicture(int n);
    void CloseCamera();
};

#endif // CAMERA_H
