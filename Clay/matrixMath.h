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
};