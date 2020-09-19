#pragma once

class Math {
    public:
        Math(){}
        
        Transformation multiply(Transformation m1, Transformation m2) {
            Transformation matrix; 
            for (int c = 0; c < 4; c++) {
                for (int r = 0; r < 4; r++) {
                    matrix.matrix[r][c] = m1.matrix[r][0] * m2.matrix[0][c] + m1.matrix[r][1] * m2.matrix[1][c] + m1.matrix[r][2] * m2.matrix[2][c]
                                 + m1.matrix[r][3] * m2.matrix[3][c];
                }
            }
            return matrix;
        }

        Vertex crossProduct(Vertex vector1, Vertex vector2)
        {
            Vertex result;
            result.x = vector1.y * vector2.z - vector1.z * vector2.y;
            result.y = vector1.z * vector2.x - vector1.x * vector2.z;
            result.z = vector1.x * vector2.y - vector1.y * vector2.x;

            return result;
        }

        float dotProduct(Vertex vector1, Vertex vector2)
        {
            return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
        }

        float vectorMagnitude(Vertex vector)
        {
            return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
        }

        Vertex normalize(Vertex vector)
        {
            Vertex normal;
            float length = vectorMagnitude(vector);
            normal.x = vector.x / length;
            normal.y = vector.y / length;
            normal.z = vector.z / length;
            return normal;
        }
};
