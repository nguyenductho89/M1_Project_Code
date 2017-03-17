//
//  main.cpp
//  MeanShiftClusteringCPP
//
//  Created by Nguyễn Đức Thọ on 2/20/17.
//  Copyright © 2017 Nguyễn Đức Thọ. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "MeanShift.h"

using namespace std;

//vector<vector<double> > load_points(const char *filename) {
//    vector<vector<double> > points;
//    FILE *fp = fopen(filename, "r");
//    char line[50];
//    while (fgets(line, sizeof(line), fp) != NULL) {
//        double x, y;
//        char *x_str = line;
//        char *y_str = line;
//        while (*y_str != '\0') {
//            if (*y_str == ',') {
//                *y_str++ = 0;
//                x = atof(x_str);
//                y = atof(y_str);
//                vector<double> point;
//                point.push_back(x);
//                point.push_back(y);
//                points.push_back(point);
//                break;
//            }
//            ++y_str;
//        }
//    }
//    fclose(fp);
//    return points;
//}

void print_points(vector<vector<double> > points){
    for(int i=0; i<points.size(); i++){
        for(int dim = 0; dim<points[i].size(); dim++) {
            printf("%f ", points[i][dim]);
        }
        printf("\n");
    }
}
double euclidean_distance1(const vector<double> &firstImage, const vector<double> &secondImage){
    double total = 0;
    for(int index=0; index<firstImage.size(); index++){
        total += (firstImage[index] - secondImage[index]) * (firstImage[index] - secondImage[index]);
    }
    return sqrt(total);
}

int main(int argc, char **argv)
{
    
    
    MeanShift *msp = new MeanShift();

    vector<double> c1 = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0};
    
    vector<double> c2 = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    vector<double> c3 = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    
    vector<double> i1 = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0 ,0 ,0 ,0};
    
    vector<double> i2 = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0};
    
    vector<double> i3 = {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};

    double c1c2 = euclidean_distance1(c1, c2);
    double c1c3 = euclidean_distance1(c1, c3);
    double c3c2 = euclidean_distance1(c3, c2);
    
    double i1i2 = euclidean_distance1(i1, i2);
    double i1i3 = euclidean_distance1(i1, i3);
    double i3i2 = euclidean_distance1(i3, i2);
    
    double i1c2 = euclidean_distance1(i1, c2);
    double i1c3 = euclidean_distance1(i1, c3);
    double i1c1 = euclidean_distance1(i1, c1);
    
    double i2c1 = euclidean_distance1(i2, c1);
    double i2c2 = euclidean_distance1(i2, c2);
    double i2c3 = euclidean_distance1(i2, c3);
    
    double i3c1 = euclidean_distance1(i3, c1);
    double i3c2 = euclidean_distance1(i3, c2);
    double i3c3 = euclidean_distance1(i3, c3);
    
    

    double kernel_bandwidth = 1;
    
    vector<vector<double> > points = {c1,c2,c3,i1,i2,i3};
    vector<Cluster> clusters = msp->cluster(points, kernel_bandwidth);
    

    
    printf("\n====================\n");
    printf("Found %lu clusters\n", clusters.size());
    printf("====================\n\n");
    
      return 0;
}
