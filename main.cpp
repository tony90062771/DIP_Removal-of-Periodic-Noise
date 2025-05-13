#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FFT1.C"
#include "FFT2.C"
#include "bmp.h"
int R[MaxBMPSizeX][MaxBMPSizeY]; 
int r[MaxBMPSizeX][MaxBMPSizeY]; 
float X[2 * MaxBMPSizeX * MaxBMPSizeY]; // �Ω�FFT���ƾ�
double fft[MaxBMPSizeX][MaxBMPSizeY][2] = {}; // �Ω�s���W�мƾ�

int main()
{
    int width, height;
    int i, j, y, z;

    open_bmp("periodic_noise2.bmp", R, R, R, width, height);

    // �إߤ@�ӯx�}�A�곡�곡�����
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            X[(i * width + j) * 2] = R[i][j];  // �곡�s�񹳯���
            X[(i * width + j) * 2 + 1] = 0;    // �곡�s��0
        }
    }

    // ����ť߸��ܴ�
    fft2(X, width, 1);

    // �W��첾
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            int new_i = (i + width / 2) % width;
            int new_j = (j + height / 2) % height;
            fft[new_i][new_j][0] = X[(i * width + j) * 2];
            fft[new_i][new_j][1] = X[(i * width + j) * 2 + 1];
        }
    }

    // �p���ƪ��W�йϹ�
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            r[i][j] = (int)(log(1000 + sqrt(pow(fft[i][j][0], 2) + pow(fft[i][j][1], 2))));
            if (r[i][j] > 255)
                r[i][j] = 255;
        }
    }
    // �W�йL�o
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            if (r[i][j] == 14) {
                for (y = -1; y <= 1; y++) {
                    for (z = -1; z <= 1; z++) {
                        int ii = (i + y + width) % width;
                        int jj = (j + z + height) % height;
                        if (abs(y) == abs(z) && y != 0) {
                            fft[ii][jj][0] *= 0.8;
                            fft[ii][jj][1] *= 0.8;
                        }
                        else if (abs(y) != abs(z)) {
                            fft[ii][jj][0] *= 0.5;
                            fft[ii][jj][1] *= 0.5;
                        }
                        else if (abs(y) == abs(z) && y == 0) {
                            fft[ii][jj][0] = 0;
                            fft[ii][jj][1] = 0;
                        }
                    }
                }
            }
        }
    }
    // �ϦV�W��첾
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            int new_i = (i + width / 2) % width;
            int new_j = (j + height / 2) % height;
            X[(new_i * width + new_j) * 2] = fft[i][j][0];
            X[(new_i * width + new_j) * 2 + 1] = fft[i][j][1];
        }
    }
    // ����ť߸����ܴ�
    fft2(X, width, -1);
    // �N���ܴ��᪺�Ȧs�J�v���x�}�O�s
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            r[i][j] = (int)X[(i * width + j) * 2];
        }
    }
    // �O�s�B�z���G�ܷs�����ɤ�
    save_bmp("new.bmp", r, r, r, width, height); 
    printf("Job Finished!\n");
    // ���� bmp �v����
    close_bmp();
    return 0;
}




