#include "headers.h"

class App : public clayGameEngine
{
public:
  App()
  {
    m_sAppName = L"Clay";
  }

  vector<object> objects;

private:
  float fTheta;

  float initialize(float fElapsedTime) {
      // REMOVE WRAPPED IMPLEMENTATIONS AT YOUR OwN RISK
        //  ==============================================
      fTheta += 1.0f * fElapsedTime;
      Fill(0, 0, c.ScreenWidth(), c.ScreenHeight(), PIXEL_SOLID, FG_BLACK);
      Input(fElapsedTime);
      init(objects);
      return fTheta;
      //  ==============================================
  }

  bool OnUserCreate()
  {
    object o("./TestCases/teapot.obj");
    objects.push_back(o);
    return true;
  }

  bool OnUserUpdate(float fElapsedTime)
  {
        float fTheta = initialize(fElapsedTime);
        objects[0].Rotate(fTheta, true, false, true, 3.0f, 2.5f);
        return true;
  }
};

int main()
{
  App clay;
  if (clay.ConstructConsole(300, 300, 1, 1))
  {
    clay.Start();
  }
}
