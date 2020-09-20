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
      float fNear = 0.3f;
      float fFar = 2500.0f;
      float fFov = 90.0f;
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

  //PointAt and LookAt algorithms taken and modified from OneLonderCoder.com
  void PointMat(Vertex &position, Vertex &target, Vertex &up)
  {
    Math m;

    //calculate forward direction
    Vertex newForward = m.normalize(m.VectorSub(target, position));

    //Calculate upward direction
    Vertex tempU = m.VectorMultiplication(newForward, m.dotProduct(up, newForward));
    Vertex newUpward = m.normalize(m.VectorSub(up, tempU));

    //Calculate right direction
    Vertex newRight = m.crossProduct(newUpward, newForward);

    //point at matrix
    mCamera[0][0] = newRight.x;
    mCamera[0][1] = newRight.y;
    mCamera[0][2] = newRight.z;
    mCamera[0][3] = 0;
    mCamera[1][0] = newUpward.x;
    mCamera[1][1] = newUpward.y;
    mCamera[1][2] = newUpward.z;
    mCamera[1][3] = 0;
    mCamera[2][0] = newForward.x;
    mCamera[2][1] = newForward.y;
    mCamera[2][2] = newForward.z;
    mCamera[2][3] = 0;
    mCamera[3][0] = position.x;
    mCamera[3][1] = position.y;
    mCamera[3][2] = position.z;
    mCamera[3][3] = 1.0f;
  }

  void doThings(float mView[4][4], float nCamera[4][4])
  {
    //idk where this shit goes: red because wrong place
    Math m1;
    Vertex vView = {0, 0, 1};
    Vertex vUp = {0, 1, 0};
    Vertex vTarget = m1.VectorAdd(vCamera, vView);
    PointMat(vCamera, vTarget, vUp);
    mView = m1.Invert(mCamera);
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


  float mView[4][4] = {0};
  float mCamera[4][4] = {0};
  float matrix[4][4] = {0};
  string name = "";

private:
  //idk where this shit goes
 // Vertex vCamera;
 // Vertex vView;
};
