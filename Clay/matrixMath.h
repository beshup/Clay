class MatrixMath {
    public: 
        MatrixMath() {

        }
        
        Transformation MultiplyMatrix(Transformation m1, Transformation m2) {
            Transformation matrix; 
            for (int c = 0; c < 4; c++) {
                for (int r = 0; r < 4; r++) {
                    matrix.matrix[r][c] = m1.matrix[r][0] * m2.matrix[0][c] + m1.matrix[r][1] * m2.matrix[1][c] + m1.matrix[r][2] * m2.matrix[2][c]
                                    + m1.matrix[r][3] * m2.matrix[3][c];
                }
            }
            return matrix;
        }


        Transformation World(string inputName, float fTheta)
        {
            Transformation world;
            world.name = inputName;
            // can't use switch here :( (directly at least)

            if (inputName == "world")
            {
                Transformation matRotZ;
                Transformation matRotX;
                Transformation matTrans;

                matTrans.translate(0.0f, 0.0f, 5.0f);
                matRotX.RotateX(fTheta);
                matRotZ.RotateZ(fTheta * 0.5f);


                world.makeIdentity();
                world = MultiplyMatrix(matRotZ, matRotX);
                world = MultiplyMatrix(world, matTrans);
            } 
            return world;
        }

         void MultiplyMatrixVector(Transformation& m, Vertex &v)
         {
             float xRes;
             float yRes;
             float zRes;
             float wRes;

             xRes = v.x * m.matrix[0][0] + v.y * m.matrix[1][0] + v.z * m.matrix[2][0] + v.w * m.matrix[3][0];
             yRes = v.x * m.matrix[0][1] + v.y * m.matrix[1][1] + v.z * m.matrix[2][1] + v.w * m.matrix[3][1];
             zRes = v.x * m.matrix[0][2] + v.y * m.matrix[1][2] + v.z * m.matrix[2][2] + v.w * m.matrix[3][2];
             // w being aritifically simulating "4th element" of input vector
             wRes = v.x * m.matrix[0][3] + v.y * m.matrix[1][3] + v.z * m.matrix[2][3] + v.w * m.matrix[3][3];

             if (m.name == "projection")
             {
                 if (v.w != 0.0f)
                 {
                     xRes /= v.w;
                     yRes /= v.w;
                     zRes /= v.w;
                 }
             }

             v.x = xRes;
             v.y = yRes;
             v.z = zRes;
             v.w = wRes;
         }

};