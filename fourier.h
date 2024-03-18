
#ifndef FOUR_FOURIER_H
#define FOUR_FOURIER_H
#include <vector>


template<class T>
std::vector<T> fourier_transform(std::vector<T> signal){
    std::vector<T> new_signal;
    for(int i = 0; i < signal.size(); i++){
        T val = signal[i];

        double angle = - (2 *3.14159 / signal.size()) * i; 
        double x_comp = 0;
        double y_comp = 0;
        for(int j = 0; j < signal.size(); j++){
            double angle_j = angle * j;

            x_comp += cos(angle_j) * signal[j];
            y_comp += sin(angle_j) * signal[j];
        }
        new_signal.push_back(sqrt(x_comp*x_comp + y_comp*y_comp));
    }
    return new_signal;
}
#endif
