#pragma once

class Transformation : public Constants
{
public:
  Transformation()
  {
  }
  // if you wish to rotate, will have to make special call

  Transformation(string inputName)
  {
    name = inputName;
    // can't use switch here :( (directly at least)
    if (inputName == "projected")
    {
      float fNear = 1.0f;
      float fFar = 500.0f;
      float fFov = 150.0f;
      float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
      // degrees to rad
      float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.141519f);

      matrix[0][0] = fAspectRatio * fFovRad;
      matrix[1][1] = fFovRad;
      matrix[2][2] = fFar / (fFar - fNear);
      matrix[3][2] = (-fFar * fNear) / (fFar - fNear);
      matrix[2][3] = 1.0f;
      matrix[3][3] = 0.0f;
      name = "Projection";
    }
  }

  void RotateX(float fTheta)
  {
    matrix[0][0] = 1;
    matrix[1][1] = cosf(fTheta * 0.5f);
    matrix[1][2] = sinf(fTheta * 0.5f);
    matrix[2][1] = -sinf(fTheta * 0.5f);
    matrix[2][2] = cosf(fTheta * 0.5f);
    matrix[3][3] = 1;
  }

  void RotateY(float fTheta) {
			matrix[0][0] = cosf(fTheta);
			matrix[0][1] = sinf(fTheta);
			matrix[1][0] = -sinf(fTheta);
			matrix[1][1] = 1.0f;
			matrix[2][2] = cosf(fTheta);
			matrix[3][3] = 1.0f;
		}
    
  void RotateZ(float fTheta)
  {

    matrix[0][0] = cosf(fTheta);
    matrix[0][1] = sinf(fTheta);
    matrix[1][0] = -sinf(fTheta);
    matrix[1][1] = cosf(fTheta);
    matrix[2][2] = 1;
    matrix[3][3] = 1;
  }

  
  void translate(float x, float y, float z) {
			matrix[0][0] = 1.0f;
			matrix[1][1] = 1.0f;
			matrix[2][2] = 1.0f;
			matrix[3][3] = 1.0f;
			matrix[3][0] = x;
			matrix[3][1] = y;
			matrix[3][2] = z;
		}

		void project(float fFov, float fAspectRatio, float fNear, float fFar) {
			float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.141519f);
			matrix[0][0] = fAspectRatio * fFovRad;
			matrix[1][1] = fFovRad;
			matrix[2][2] = fFar / (fFar - fNear);
			matrix[3][2] = (-fFar * fNear) / (fFar - fNear);
			matrix[2][3] = 1.0f;
			matrix[3][3] = 0.0f;
		}

		void makeIdentity() {
			matrix[0][0] = 1.0f;
			matrix[1][1] = 1.0f;
			matrix[2][2] = 1.0f;
			matrix[3][3] = 1.0f;
		}


  float matrix[4][4] = {0};
  string name = "";

private:
  //idk where this shit goes
 // Vertex vCamera;
 // Vertex vView;
};
