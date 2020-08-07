// Fill out your copyright notice in the Description page of Project Settings.


#include "NoiseComponent.h"

FastNoise UNoiseComponent::NoiseModule = FastNoise();

void UNoiseComponent::Init() {
	NoiseModule.SetNoiseType(NoiseType::Perlin);
}


float UNoiseComponent::GetPerlin(int x, int y, int z) {

	float Noise = z - NoiseModule.GetNoise(x, y) * 10.0f - (10.0f / 2.0f);
	
	if (Noise > 10.0f) {
		Noise = 10.0f;
	}
	if (Noise < -10.0f) {
		Noise = -10.0f;
	}
	Noise /= 10;

	if (Noise < -1) {
		Noise = -1;
	}
	if (Noise > 1) {
		Noise = 1;
	}
	Noise = (Noise + 1) * 0.5f;
	
	return Noise;
}
