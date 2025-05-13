#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FFT1.C"
#include "FFT2.C"
#include "bmp.h"
int R[MaxBMPSizeX][MaxBMPSizeY]; 
int r[MaxBMPSizeX][MaxBMPSizeY]; 
float X[2 * MaxBMPSizeX * MaxBMPSizeY]; // 用於FFT的數據
double fft[MaxBMPSizeX][MaxBMPSizeY][2] = {}; // 用於存放頻譜數據

int main()
{
    int width, height;
    int i, j, y, z;

    open_bmp("periodic_noise2.bmp", R, R, R, width, height);

    // 建立一個矩陣，實部虛部交錯放
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            X[(i * width + j) * 2] = R[i][j];  // 實部存放像素值
            X[(i * width + j) * 2 + 1] = 0;    // 虛部存放0
        }
    }

    // 執行傅立葉變換
    fft2(X, width, 1);

    // 頻域位移
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            int new_i = (i + width / 2) % width;
            int new_j = (j + height / 2) % height;
            fft[new_i][new_j][0] = X[(i * width + j) * 2];
            fft[new_i][new_j][1] = X[(i * width + j) * 2 + 1];
        }
    }

    // 計算對數的頻譜圖像
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            r[i][j] = (int)(log(1000 + sqrt(pow(fft[i][j][0], 2) + pow(fft[i][j][1], 2))));
            if (r[i][j] > 255)
                r[i][j] = 255;
        }
    }
    // 頻譜過濾
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
    // 反向頻域位移
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            int new_i = (i + width / 2) % width;
            int new_j = (j + height / 2) % height;
            X[(new_i * width + new_j) * 2] = fft[i][j][0];
            X[(new_i * width + new_j) * 2 + 1] = fft[i][j][1];
        }
    }
    // 執行傅立葉反變換
    fft2(X, width, -1);
    // 將反變換後的值存入影像矩陣保存
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            r[i][j] = (int)X[(i * width + j) * 2];
        }
    }
    // 保存處理結果至新的圖檔中
    save_bmp("new.bmp", r, r, r, width, height); 
    printf("Job Finished!\n");
    // 關閉 bmp 影像檔
    close_bmp();
    return 0;
}




