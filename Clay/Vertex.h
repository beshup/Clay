#pragma once

class Vertex {
public:
    Vertex() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 1.0f;
    }
    Vertex(float x1, float y1) {
        x = x1;
        y = y1;
    }
    Vertex(float x1, float y1, float z1) {
        x = x1;
        y = y1;
        z = z1;
        w = 1.0f;
    }

    void MultiplyMatrixVector(Transformation &m) {
        float xRes;
        float yRes;
        float zRes;
        float wRes;

        xRes = x * m.matrix[0][0] + y * m.matrix[1][0] + z * m.matrix[2][0] + w * m.matrix[3][0];
        yRes = x * m.matrix[0][1] + y * m.matrix[1][1] + z * m.matrix[2][1] + w * m.matrix[3][1];
        zRes = x * m.matrix[0][2] + y * m.matrix[1][2] + z * m.matrix[2][2] + w * m.matrix[3][2];
        // w being aritifically simulating "4th element" of input vector
        wRes = x * m.matrix[0][3] + y * m.matrix[1][3] + z * m.matrix[2][3] + w * m.matrix[3][3];

        if (m.name == "projection") {
            if (w != 0.0f) {
                xRes /= w;
                yRes /= w;
                zRes /= w;
            }
        }
        
        x = xRes;
        y = yRes;
        z = zRes;
        w = wRes;
    }

    float x, y, z = 0.0f;
    float w = 1.0f;
};
