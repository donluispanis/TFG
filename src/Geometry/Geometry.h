#pragma once

#include <vector>
#include "../ClassicDemoTemplate/ClassicDemoTemplate.h"
#include "../Utils/Pixel.h"

struct IWindowManager;

class GeometryDemo : public ClassicDemoTemplate
{
  public:
    GeometryDemo(){};
    virtual ~GeometryDemo(){};

  private:
    struct Point2D
    {
        float X;
        float Y;
    };
    struct Point3D
    {
        float X;
        float Y;
        float Z;
    };
    struct Object3D
    {
        std::vector<Point3D> points;
        std::vector<Point2D> indexes;
        std::vector<Point2D> projectedPoints;
    };

    virtual bool Init() override;
    virtual bool Update(float deltaTime) override;
    virtual bool Destroy() override;

    void Generate2DProjection(Object3D &object);
    void TranslateObject(Object3D &object, Point3D offset);
    void ScaleObject(Object3D &object, Point3D scale);
    void Rotate3DObjectAroundXAxis(Object3D &object, float angle);
    void Rotate3DObjectAroundYAxis(Object3D &object, float angle);
    void Rotate3DObjectAroundZAxis(Object3D &object, float angle);
    void RenderObject(Object3D object, const Pixel &colour);

    Pixel *pixels;
    int width, height;
    IWindowManager *windowManager;

    Object3D object;
};