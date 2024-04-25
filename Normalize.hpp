#pragma once
#include "Effect.hpp"

 template <typename T>
 class Normalize 
 {
 private:
     T coefficient;
     T border;

     void getCoefficient(const Sound<T>* sound);
 public:
     Normalize() = default;
     Normalize(const Sound<T>* sound, T border = -1);
     T operator()(T sample, unsigned index) const;
 };

 template<typename T>
 inline void Normalize<T>::getCoefficient(const Sound<T>* sound)
 {
     T maxNumber = std::abs((*sound)[0]);
     unsigned size = sound->getSampleLength();
     for (size_t i = 1; i < size; i++)
     {
         if (maxNumber < std::abs((*sound)[i]))
         {
             maxNumber = std::abs((*sound)[i]);
         }
     }

     coefficient = std::abs(border) / maxNumber;
 }

 template<typename T>
 inline Normalize<T>::Normalize(const Sound<T>* sound, T border) : border(border)
 {
     if (sound == nullptr) {
         throw std::invalid_argument("Sound must be different than nullptr");
     }
     getCoefficient(sound);
 }

 template<typename T>
 inline T Normalize<T>::operator()(T sample, unsigned index) const
 {
     return sample * coefficient;
 }
